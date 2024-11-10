#pragma once

namespace ed {
    enum packet_ids {
        id_get_module_info = 0x01,
        id_read_memory = 0x02,
        id_error_while_work = 0xCC,
        id_unknown_packet = 0xFF,
    };

    class driver_interface {
        int sock = -1;

        protected:
            static void client_thread(void* sock) {
                unsigned char data[4096] = {};
                while(sock) {
                    auto length = recv(int(ULONG_PTR(sock)), (void*)&data, 4096, 0);
                    if(length <= 0) return;

                    switch(data[0]) {
                        case ed::id_get_module_info: {
                            if(length < 7) continue;
                            char module_name[256] = { 0 };
                            unsigned long long base = 0;
                            unsigned long long size = 0;
                            unsigned int process_id = *(unsigned int*)&data[2];
                            memcpy(module_name, &data[6], length - 6);

                            if(!NT_SUCCESS(ed::get_process_module_info(process_id, module_name, base, size, data[1]))) {
                                unsigned char request[2] = { ed::id_error_while_work, data[0] };
                                send(int(ULONG_PTR(sock)), request, sizeof(request), 0);
                                continue;
                            };

                            unsigned char request[17] = { ed::id_get_module_info };
                            *(unsigned long long*)&request[1] = base;
                            *(unsigned long long*)&request[8] = size;
                            send(int(ULONG_PTR(sock)), request, sizeof(request), 0);
                            break;
                        };

                        case ed::id_read_memory: {
                            if(length < 20) continue;
                            unsigned int process_id = *(unsigned int*)&data[1];
                            unsigned long long address = *(unsigned long long*)&data[5];
                            unsigned long long size = *(unsigned long long*)&data[13];

                            unsigned char* request = (unsigned char*)ExAllocatePool(NonPagedPool, size + 1);
                            request[0] = ed::id_read_memory;
                            ed::read_virtual_memory(process_id, request + 1, address, size);
                            send(int(ULONG_PTR(sock)), request, size + 1, 0);
                            ExFreePool(request);
                            break;
                        };

                        default: {
                            unsigned char request[2] = { ed::id_unknown_packet, data[0] };
                            send(int(ULONG_PTR(sock)), request, sizeof(request), 0);
                            break;
                        };
                    };
                };
            };

            static void server_thread(void* sock) {
                sockaddr socket_addr = { };
                socklen_t socket_length = 0;

                while(sock) {
                    const auto client = accept(int(ULONG_PTR(sock)), &socket_addr, &socket_length);
                    if(client <= 0) break;

                    HANDLE client_thread_handle = 0;
                    if(!NT_SUCCESS(PsCreateSystemThread(&client_thread_handle, GENERIC_ALL, 0, 0, 0, ed::driver_interface::client_thread, (void*)client))) return;
                    ZwClose(client_thread_handle);
                };
            };

        public:
            driver_interface() = default;
            NTSTATUS init() {
                SOCKADDR_IN address = { };
                address.sin_family = AF_INET;
                address.sin_port = htons(1337);

                sock = socket_listen(AF_INET, SOCK_STREAM, 0);
                if(sock <= 0) return STATUS_INVALID_PARAMETER;
                if(bind(sock, (SOCKADDR*)&address, sizeof(address)) == -1) return STATUS_INVALID_PARAMETER_1;
                if(listen(sock, 128) == -1) return STATUS_INVALID_PARAMETER_2;

                HANDLE socket_thread = 0;
                PsCreateSystemThread(&socket_thread, GENERIC_ALL, 0, 0, 0, ed::driver_interface::server_thread, (void*)sock);
                return ZwClose(socket_thread);
            };
    };
};