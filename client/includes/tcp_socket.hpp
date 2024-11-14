#pragma once

#define sector_size 3
namespace ed {
    class tcp_socket {
        int sock = -1;

        public:
            tcp_socket() = default;
            ~tcp_socket() {
                closesocket(sock);
                WSACleanup();
            };

            unsigned char* process(unsigned char* data, unsigned long long length) {
                if(!data) return 0;

                for(unsigned int i = 0; i < length; i++) data[i] ^= 0xC9;
                unsigned int sector_count = length / sector_size;
                unsigned char sector[sector_size];
                if(!((sector_count / length) % 2)) sector_count--;
                
                for(unsigned int i = 0; i < (sector_count * sector_size); i += sector_size * 2) {
                    if(i + sector_size > length) continue;
                    memcpy(&sector[0], &data[i], sector_size);
                    
                    memcpy(&data[i], &data[i + sector_size], sector_size);
                    memcpy(&data[i + sector_size], &sector[0], sector_size);
                };
                
                return data;
            };

            bool try_connect(std::string to, unsigned short port_to) {
                WSADATA wsaData = { 0 };
                WSAStartup(MAKEWORD(2, 2), &wsaData);

                sock = socket(AF_INET, SOCK_STREAM, 0);
                if(sock == -1) return false;

                DWORD timeout = 3000;
                setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

                struct sockaddr_in addr = { 0 };
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = inet_addr(to.c_str());
                addr.sin_port = htons(port_to);

                return connect(sock, (sockaddr*)&addr, 16) != -1;
            };

            bool try_send(unsigned char* data, unsigned short length) {
                if(!data) return false;
                return send(sock, (char*)this->process(data, length), length, 0);
            };

            auto get() { return sock; };
    };
};