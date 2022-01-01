#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <time.h>
#include <stdio.h> //fprintf

int req (int sock, char* buffer, const char* message, int size) {
    int response = -1;

    send (sock, message, size, 0);
    //std::cout << "\nMessage sent: " << message <<"\n";
    response = read (sock, buffer, 1024);
    
    //std::cout << "Server 1 response: " << buffer << ", \"" << buffer[0] << "\"\n";
    
    // Probably need to implement a better check for the exit lols

    return response;
}

int main (int argc, char** argv) {
    
    int sock = -1, exit;
    struct sockaddr_in serv_addr;
    std::string message;
    char buffer[1024] = {0};

    if (argc != 3) {
        std::cout << "Please include IP and Port of server\n";
        return -1;
    }

    std::string IP (argv[1]);
    std::string Port (argv[2]);
    int portInt = std::stoi(Port,nullptr,10);

    std::cout << "Client is running\n";
    std::cout << "Received IP and port#:\n";
    std::cout << IP << ", " << Port << "\n";

    // Creates socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "\nFailed to create socket\n";
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portInt);
    // Converts IP address to binary
    if (inet_pton(AF_INET, IP.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cout << "\nIP failed to convert\n";
    }

    // Attempts to connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "\nFailed to connect\n";
    }

    message = "Hello!";

    while (message != "c:EXIT") {
        std::cout << "\t1. Translate\n";
        std::cout << "\t2. Convert\n";
        std::cout << "\t3. Vote\n";
        std::cout << "\tEnter \":EXIT\" to exit\n";
        std::cout << "Please choose a service:\n";
        std::cin >> message;
        message = "c" + message;    // Appends a flag character at the beginning of the message to
                                    // notify the servers that this message was from the client
        exit = req(sock, buffer, message.c_str(), message.size());
        
        //std::cout << "Server 2 response: " << buffer << ", \"" << buffer[0] << "\"\n";
        
        // Here I can start reading buffer

        if (buffer[0] == 't') {          // Server replies with this and prompts user translate stuff
            std::cout << "Translate:\n";

            // Prompts user for word
            std::cout << "> Enter an English word: ";
            std::cin >> message;
            message = "c" + message;

            // Sends request to user and prints reply from server
            // Server should be in TRANSLATE state
            exit = req(sock, buffer, message.c_str(), message.size());

            std::cout << "> French translation: " << buffer << "\n";

        } else if (buffer[0] == 'c') {   // Server replies with this and prompts user convert stuff
            std::cout << "Convert:\n";

            // Prompts user for value, currency, and desired currency
            // May do this line by line because I'm lazy lol
            std::cout << "> Enter value:\n";
            std::cin >> message;
            message = "c" + message;
            // Sends request to user and prints reply from server
            // Server should be in CONVERT state
            exit = req(sock, buffer, message.c_str(), message.size());

            std::cout << "> Enter current currency:\n";
            std::cin >> message;
            message = "c" + message;
            // Sends request to user and prints reply from server
            // Server should be in CONVERT state
            exit = req(sock, buffer, message.c_str(), message.size());

            std::cout << "> Enter desired currency:\n";
            std::cin >> message;
            message = "c" + message;
            // Sends request to user and prints reply from server
            // Server should be in CONVERT state
            exit = req(sock, buffer, message.c_str(), message.size());

            // This output is going to be formatted at the server side I guess...
            std::cout << "Output: " << buffer << "\n";

        } else if (buffer[0] == 'v') {   // Server replies with this and prompts user vote stuff
            std::cout << "Vote:\n";
            std::string extraString = "";

            int key = 0;    // Single digit key extracted as the first character of the message
                            // from the mircoserver
            int response = 0;
            // Looks like I'm going to need to ask the microservers for a list of candidates lols
            // Server should be in VOTE state
            message = "please...";
            message = "c" + message;
            exit = req(sock, buffer, message.c_str(), message.size());
            //std::cout << "LIST + KEY\n" << buffer << "\n";

            // converts the first character into an int
            key = buffer[0] - 48;
            
            buffer[0] = '\n';

            std::cout << "Here are the list of candidates:\n";
            std::cout << buffer << "\n";
            std::cout << "Enter ID of candidate you wish to vote for:\n";
            std::cin >> extraString;

            // This part sucks
            response = extraString[0] - 48;
            response *= key;
            
            //std::cout << "Message to server: " << response << "\n";

            extraString = std::to_string(response);

            for (int i = 0; i < 1024; i++) buffer[i] = 0;
            exit = req(sock, buffer, extraString.c_str(), extraString.size());

            std::cout << buffer <<"\n";

            std::cin >> extraString;

            // This is where we ask them if they want to see results
            for (int i = 0; i < 1024; i++) buffer[i] = 0;
            exit = req(sock, buffer, extraString.c_str(), extraString.size());

            std::cout << buffer <<"\n";

        } else if (buffer[0] == 'f') {   // Server replies with a fail message
            if (message != "c:EXIT") std::cout << "ERROR: please enter a valid request\n";
        } else {                        // Server replies with broken message. Shouldn't be broken anyway lol since this is TCP
            if (message != "c:EXIT") std::cout << "ERROR: Server reply has failed\n";
        }

        // Flush buffer after each use
        for (int i = 0; i < 1024; i++) buffer[i] = 0;

    }

    return 0;
}