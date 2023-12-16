#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>

#include <mutex>

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

    //std::cout << response << "\n";

    std::string parsed = ParseResponse(response);
    std::cout << parsed << '\n';
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

/*
void CreateNamedPipeServer() {
    LPCWSTR pipeName = L"\\\\.\\pipe\\Lab9Pipe";

     //Explicitly delete the existing named pipe file
    if (!DeleteFile(pipeName)) {
        DWORD error = GetLastError();
        if (error != ERROR_FILE_NOT_FOUND) {
            std::wcerr << L"Error deleting named pipe file. Error code: " << error << std::endl;
            return;
        }
    }

    HANDLE hPipe = CreateNamedPipe(
        pipeName,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        4096,
        4096,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Error creating named pipe. Error code: " << GetLastError() << std::endl;
        return;
    }

    std::wcout << L"Named pipe server created successfully." << std::endl;

    while (true) {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
            std::wcout << L"Client connected to the named pipe." << std::endl;
            const WCHAR* message = StringToWchar(GenerateJoke("Write me a joke about software engineering"));
            DWORD bytesWritten;

            if (WriteFile(hPipe, message, wcslen(message) * sizeof(WCHAR), &bytesWritten, NULL) != FALSE) {
                std::wcout << L"Message sent to the client." << std::endl;
            }
            else {
                std::wcerr << L"Error writing to the pipe. Error code: " << GetLastError() << std::endl;
            }
            DisconnectNamedPipe(hPipe);
        }

        Sleep(10000);
    }

    CloseHandle(hPipe);
}
*/

//void HandleClient(HANDLE hPipe) {
//    const WCHAR* message = StringToWchar(GenerateJoke("Write me a joke about software engineering"));
//    DWORD bytesWritten;
//
//    if (WriteFile(hPipe, message, wcslen(message) * sizeof(WCHAR), &bytesWritten, NULL) != FALSE) {
//        std::wcout << L"Message sent to the client." << std::endl;
//    }
//    else {
//        std::wcerr << L"Error writing to the pipe. Error code: " << GetLastError() << std::endl;
//    }
//
//    DisconnectNamedPipe(hPipe);
//    CloseHandle(hPipe);
//}
/*
void HandleClient(HANDLE hPipe) {
    const WCHAR* message = L"Hello from the server!";
    DWORD bytesWritten;

    if (WriteFile(hPipe, message, wcslen(message) * sizeof(WCHAR), &bytesWritten, NULL) != FALSE) {
        std::wcout << L"Message sent to the client." << std::endl;
    }
    else {
        std::wcerr << L"Error writing to the pipe. Error code: " << GetLastError() << std::endl;
    }

    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}

void CreateNamedPipeServer() {
    LPCWSTR pipeName = L"\\\\.\\pipe\\Lab9Pipe";

    if (!DeleteFile(pipeName)) {
        DWORD error = GetLastError();
        if (error != ERROR_FILE_NOT_FOUND) {
            std::wcerr << L"Error deleting named pipe file. Error code: " << error << std::endl;
            return;
        }
    }

    std::vector<std::thread> threads;

    while (true) {
        HANDLE hPipe = CreateNamedPipe(
            pipeName,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            4096,
            4096,
            NMPWAIT_USE_DEFAULT_WAIT,
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::wcerr << L"Error creating named pipe. Error code: " << GetLastError() << std::endl;
            return;
        }

        std::wcout << L"Named pipe server created successfully." << std::endl;

        if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
            std::wcout << L"Client connected to the named pipe." << std::endl;

            // Create a new thread to handle the client
            threads.emplace_back(HandleClient, hPipe);
        }
        else {
            std::wcerr << L"Error connecting to the pipe. Error code: " << GetLastError() << std::endl;
            CloseHandle(hPipe);
        }

        std::this_thread::sleep_for(std::chrono::seconds(10)); // Wait for 10 seconds
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
}*/

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

// Mutex to protect access to shared data
//std::mutex clientMutex;

// Function to send jokes to clients
void SendJokesToClients(SOCKET* clientSockets, int& clientCount) {
    while (true) {
        // Generate a new joke
        std::string joke = GenerateJoke("Write me a joke about software engineering");

        // Lock before accessing client data
        //std::lock_guard<std::mutex> lock(clientMutex);

        // Send the joke to all clients
        for (int i = 0; i < clientCount; ++i) {
            int result = send(clientSockets[i], joke.c_str(), static_cast<int>(joke.length()), 0);
            if (result == SOCKET_ERROR) {
                std::cerr << "Error sending joke to client " << i << ": " << WSAGetLastError() << std::endl;
            }
        }

        // Sleep for 30 seconds (for testing purposes)
        std::this_thread::sleep_for(std::chrono::seconds(5));
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

        // Lock before accessing client data
        //std::lock_guard<std::mutex> lock(clientMutex);

        if (clientCount < MAX_CLIENTS) {
            clientSockets[clientCount] = clientSocket;
            clientCount++;

            std::cout << "New client connected. Client count: " << clientCount << std::endl;

        }
        else {
            std::cerr << "Too many clients. Connection rejected." << std::endl;
            closesocket(clientSocket);
        }

        // Check for client disconnections
        for (int i = 0; i < clientCount; ++i) {
            char buffer[1];
            int result = recv(clientSockets[i], buffer, sizeof(buffer), MSG_PEEK);
            if (result != 0) {
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

    // Join the jokeThread before exiting the program
    jokeThread.join();

    closesocket(listenSocket);
    WSACleanup();



    return 0;
}