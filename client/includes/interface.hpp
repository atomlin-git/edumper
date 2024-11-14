#pragma once

namespace ed {
    enum packet_ids {
        id_get_module_info = 0x01,
        id_read_memory = 0x02,
        id_error_while_work = 0xCC,
        id_unknown_packet = 0xFF,
    };

    class driver_interface : virtual tcp_socket {
        unsigned int process_id = -1;
        
        unsigned char* receive(ed::packet_ids need_id, unsigned long long size = 4096) {
            unsigned char* buffer = new unsigned char[size];
            unsigned long long offset = 0;

            while(this) {
                auto length = recv(this->get(), (char*)buffer + offset, size, 0);
                if(length <= 0) return 0;

                offset += length;
                if(offset >= size) {
                    memcpy(buffer, this->process(buffer, length), length);
                    if(buffer[0] != need_id) return 0;
                    return (unsigned char*)buffer + 1;
                }
            };

            delete[] buffer;
            return 0;
        };

        public:
            driver_interface() = default;
            bool init_process(unsigned int pid) {
                process_id = pid;
                return this->try_connect("127.0.0.1", 1337);
            };

            template<typename type>
                type read_memory(unsigned long long address, unsigned long long size) {
                    unsigned char request[20] = { ed::id_read_memory };
                    *(unsigned int*)&request[1] = this->process_id;
                    *(unsigned long long*)&request[5] = address;
                    *(unsigned long long*)&request[13] = size;
                    this->try_send(request, 20);
                    return (type)this->receive(ed::id_read_memory, size + 1);
                };

            std::pair<unsigned long long, unsigned long long> get_module_info(std::string module, bool wow64 = false) {
                if(module.empty()) return { -1, -1 };
                unsigned char request[4096] = { ed::id_get_module_info, wow64 };
                *(unsigned int*)&request[2] = this->process_id;
                memcpy(&request[6], module.c_str(), module.length());
                this->try_send(request, 7 + module.length());
                auto result = this->receive(ed::id_get_module_info, 17);
                return { result ? *(unsigned long long*)&result[0] : -1, result ? *(unsigned long long*)&result[6] : -1 };
            };
    };
};