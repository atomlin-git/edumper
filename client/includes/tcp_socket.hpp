#pragma once

namespace ed {
    class tcp_socket {
        int sock = -1;

        public:
            tcp_socket() = default;
            ~tcp_socket() {
                closesocket(sock);
                WSACleanup();
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
                return send(sock, (char*)data, length, 0);
            };

            auto get() { return sock; };
    };
};