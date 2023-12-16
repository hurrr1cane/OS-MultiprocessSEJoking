#include <windows.h>
#include <iostream>
#include <thread>

#define CURL_STATICLIB

#include <curl/curl.h>
#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

std::string ExtractJsonValue(const std::string& json, const std::string& key) {
    size_t keyStart = json.find("\"" + key + "\":");
    if (keyStart == std::string::npos) {
        return "";
    }

    size_t valueStart = json.find("\"", keyStart + key.length() + 3) + 1;
    size_t valueEnd = json.find("\"", valueStart);

    while (valueEnd != std::string::npos && json[valueEnd - 1] == '\\') {
        // Handle escaped quote, find the next quote
        valueEnd = json.find("\"", valueEnd + 1);
    }

    if (valueStart != std::string::npos && valueEnd != std::string::npos) {
        std::string extractedValue = json.substr(valueStart, valueEnd - valueStart);
        return extractedValue;
    }

    return "";
}

std::string ParseResponse(const std::string& response) {
    std::string content = ExtractJsonValue(response, "content");
    // Replace escaped newlines and quotes
    content = ReplaceAll(content, "\\n", "\n");
    content = ReplaceAll(content, "\\\"", "\"");
    return content.empty() ? "Why did Levus send the student to re-study? Because he could not repeat Boehm's seventh law." : content;
}

std::string GenerateJoke(const std::string& prompt) {
    std::string apiKey = "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";
    std::string apiUrl = "https://api.openai.com/v1/chat/completions";

    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        std::string requestBody = "{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\",\"content\" :\"" + prompt + "\"}]}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Curl request failed: " << curl_easy_strerror(res) << std::endl;
            return "Curl request failed.";
        }
    }

    //std::cout << response << '\n';

    std::string parsed = ParseResponse(response);
    return parsed;
}

const WCHAR* StringToWchar(const std::string& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (size == 0) {
        // Handle error
        return nullptr;
    }

    WCHAR* wideStr = new WCHAR[size];
    if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wideStr, size) == 0) {
        // Handle error
        delete[] wideStr;
        return nullptr;
    }

    return wideStr;
}

#define DEFAULT_PORT "27015"

// Function to send jokes to clients
void SendJokesToClients(SOCKET* clientSockets, int& clientCount) {
    while (true) {
        // Generate a new joke
        std::string joke = GenerateJoke("Write me a joke about software engineering");

        std::cout << "Generated joke: " << ReplaceAll(joke, "\n", " ") << '\n';

        // Send the joke to all clients
        for (int i = 0; i < clientCount; ++i) {
            std::cout << "Sending joke to " << i << "' client..." << '\n';

            int result = send(clientSockets[i], joke.c_str(), static_cast<int>(joke.length()), 0);
            if (result == SOCKET_ERROR) {
                std::cerr << "Error sending joke to client " << i << ": " << WSAGetLastError() << std::endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(20));
    }
}

// Function to check for disconnected clients and remove them
void CheckForDisconnectedClients(SOCKET* clientSockets, int& clientCount) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Adjust the sleep duration as needed

        for (int i = 0; i < clientCount; ++i) {
            char buffer[1];
            int result = recv(clientSockets[i], buffer, sizeof(buffer), MSG_PEEK);
            if (result == 0 || (result == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)) {
                // Client disconnected
                std::cout << "Client " << i << " disconnected." << std::endl;

                // Close the socket
                closesocket(clientSockets[i]);

                // Remove the client from the array
                for (int j = i; j < clientCount - 1; ++j) {
                    clientSockets[j] = clientSockets[j + 1];
                }

                clientCount--;
            }
        }
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(DEFAULT_PORT));
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << DEFAULT_PORT << std::endl;

    const int MAX_CLIENTS = 128;
    SOCKET clientSockets[MAX_CLIENTS];
    int clientCount = 0;

    // Create a thread to check for disconnected clients
    std::thread disconnectThread(CheckForDisconnectedClients, clientSockets, std::ref(clientCount));

    // Create a thread for sending jokes to clients
    std::thread jokeThread(SendJokesToClients, clientSockets, std::ref(clientCount));

    while (true) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        if (clientCount < MAX_CLIENTS) {
            clientSockets[clientCount] = clientSocket;
            clientCount++;

            std::cout << "New client connected. Client count: " << clientCount << std::endl;
        }
        else {
            std::cerr << "Too many clients. Connection rejected." << std::endl;
            closesocket(clientSocket);
        }
    }

    jokeThread.join();

    closesocket(listenSocket);
    WSACleanup();

    return 0;
}