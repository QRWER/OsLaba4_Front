#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include<winsock2.h>
#pragma warning(disable: 4996)//для inet_addr

bool printCausedBy(int Result, const char* nameOfOper) {
    if (!Result) {
        std::cout << "Connection closed.\n";
        return false;
    }
    else if (Result < 0) {
        std::cout << nameOfOper;
        std::cerr << " failed: " << WSAGetLastError() << '\n';
        return false;
    }
    return true;
}

void Error(std::string msg, SOCKET listen){
    std::cout << msg << WSAGetLastError() << '\n';
    closesocket(listen);
    WSACleanup();
}

int main() {
    //Загрузка библиотеки
    WSAData wsaData{}; //создаем структуру для загрузки
    WORD DLLVersion = MAKEWORD(2, 1); //запрашиваемая версия библиотеки winsock
    if (WSAStartup(DLLVersion, &wsaData) != 0) { //проверка подключения библиотеки
        std::cerr << "Error: failed to link library.\n";
        return 1;
    }
    //Заполняем информацию об адресе сокета
    SOCKADDR_IN addr;
    int sizeOfAddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //адрес
    addr.sin_port = htons(1111); //порт
    addr.sin_family = AF_INET; //семейство протоколов
    //сокет для прослушивания порта
    SOCKET Connection = socket(AF_INET, SOCK_STREAM, 0);
    //проверка на подключение к серверу
    if (connect(Connection, (SOCKADDR*)&addr, sizeOfAddr)) {
        std::cerr << "Error: failed connect to the server.\n";
        return 1;
    }
    std::cout << "Server connection established.\n";
    while(true) {
        char buffer[2048];
        std::cout << "Enter the sequence: ";
        std::cin >> buffer;
        printCausedBy(send(Connection, buffer, 2048, 0), "Send");
        printCausedBy(recv(Connection, buffer, 2048, 0), "Recv");
        std::cout << buffer;
    }
}