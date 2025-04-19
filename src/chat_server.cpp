#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>

#define PORT 9000

std::vector<int> clients;
std::mutex clients_mutex;

void broadcast(const std::string& message, int sender) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client : clients) {
        if (client != sender) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_socket) {
    char buffer[1024];
    while (true) {
        int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        std::string msg = buffer;
        broadcast(msg, client_socket);
    }

    close(client_socket);
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, SOMAXCONN);
    std::cout << "💬 Chat server started on port 9000...\n";

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_socket);
        }
        std::thread(handle_client, client_socket).detach();
    }

    return 0;
}
