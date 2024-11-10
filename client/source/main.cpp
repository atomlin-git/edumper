#include "headers.hpp"

ed::driver_interface driver;

int main(unsigned int count, char *args[]) {
    std::unordered_map<std::string, std::string> arg_map;
    if(count) {
        for(unsigned int i = 1; i < count - 1; i += 2)
            arg_map[args[i]] = args[i + 1];
    };

    if(arg_map["-process"].empty() || arg_map["-module"].empty()) return printf("use: -process 1337 -module samp.exe\n");
    if(!driver.init_process(std::atoi(arg_map["-process"].c_str()))) return printf("no connection with driver\n");

    auto module_info = driver.get_module_info(arg_map["-module"], !arg_map["-x64"].empty());
    if(module_info.first == -1 && module_info.second == -1) return printf("timeout or other problem with receive module_info from driver\n");
    
    auto dos = driver.read_memory<IMAGE_DOS_HEADER*>(module_info.first, sizeof(IMAGE_DOS_HEADER));
    if(!dos) return printf("timeout or other problem with receive module dos from driver\n");
    if(dos->e_magic != IMAGE_DOS_SIGNATURE) return printf("module have bad magic! (%x, need %x)\n", dos->e_magic, IMAGE_DOS_SIGNATURE);

    auto nt = driver.read_memory<IMAGE_NT_HEADERS*>(module_info.first + dos->e_lfanew, sizeof(IMAGE_NT_HEADERS));
    if(!nt) return printf("timeout or other problem with receive module nt from driver\n");
    if(nt->Signature != IMAGE_NT_SIGNATURE) return printf("module have bad nt signature! (%x, need %x)\n", nt->Signature, IMAGE_NT_SIGNATURE);

    auto main_buffer = driver.read_memory<unsigned char*>(module_info.first, nt->OptionalHeader.SizeOfImage);
    PIMAGE_DOS_HEADER new_dos = reinterpret_cast<PIMAGE_DOS_HEADER>(main_buffer);
    PIMAGE_NT_HEADERS new_nt = reinterpret_cast<PIMAGE_NT_HEADERS>(main_buffer + dos->e_lfanew);
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(new_nt);
    for(unsigned int i = 0; i < new_nt->FileHeader.NumberOfSections; i++, section++) {
		section->SizeOfRawData = section->Misc.VirtualSize;
		section->PointerToRawData = section->VirtualAddress;
	};

    std::ofstream dumped(arg_map["-module"], std::ios::binary);
	dumped.write((char*)main_buffer, nt->OptionalHeader.SizeOfImage);
	dumped.close();

    return 1;
};