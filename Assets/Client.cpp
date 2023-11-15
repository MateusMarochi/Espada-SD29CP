#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET s;
    struct sockaddr_in server;

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cout << "Failed. Error Code : " << WSAGetLastError();
        return 1;
    }

    // Criar o socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        std::cout << "Could not create socket : " << WSAGetLastError();
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8052);

    // Conectar ao servidor remoto
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cout << "Connect error";
        return 1;
    }

    std::ifstream file("gy-91-log.txt");
    std::string line;
    long long lastTime = 0;

    // Ler e enviar dados do arquivo
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        long long timeMillis;
        char comma;

        ss >> timeMillis;
        ss >> comma; // Ignorar a vírgula

        // Calcular o atraso necessário
        long long delay = timeMillis - lastTime;
        lastTime = timeMillis;

        // Aguardar o tempo necessário
        Sleep(delay);

        // Enviar a linha
        if (send(s, line.c_str(), line.size(), 0) < 0) {
            std::cout << "Send failed";
            break;
        }
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
