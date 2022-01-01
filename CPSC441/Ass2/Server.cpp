// C++ libraries
#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <unordered_map>

// C libraries
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

// Other useful macros
#define IPv4 "136.159.5.25" // Gotta set this manually everytime I connect to the server ```curl ifconfig.me```
#define translatePort 9842
#define convertPort 3213
#define votePort 2542
#define mainPort 60003

// While debugging, just call ```killall Ser``` to end all processes

typedef struct kiddos {
    pid_t tr;
    pid_t cv;
    pid_t vo;
} kiddos;

int translateFunc() {

    //std::cout << "\nTranslate\n";

    std::unordered_map<std::string, std::string> words =
        {{"Hello","Bonjour"},
        {"Bye","Au revoir"},
        {"Map","Carte"},
        {"Packet","Paquet"},
        {"Pain","La douleur"}};

    int sock;
    struct sockaddr_in serv_data;
    struct sockaddr_in clie_data;
    struct sockaddr *client;

    sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        std::cout << "Failed to create TRANSLATE socket\n";
        return 1; 
    }

    //Init memory
    memset(&serv_data, 0, sizeof(sockaddr_in));
    serv_data.sin_family = AF_INET;
    serv_data.sin_port = htons(translatePort);
    serv_data.sin_addr.s_addr = INADDR_ANY;

    client = (struct sockaddr *) &clie_data;

    //Give socket local address defined in server
    if (bind(sock,
             (const struct sockaddr *)&serv_data,
             sizeof(sockaddr_in)) < 0)
    {
        std::cout << "Failed to bind TRANSLATE socket\n";
        close(sock);
        return 1;
    }

    int readSize;
    int testInt = sizeof(sockaddr_in);
    char buffer[1024] = {0};

    // Runs indefinitely
    while (1) {
        std::string message = "Translate send\n";

        readSize = recvfrom(sock, buffer, 1024, MSG_WAITALL, (struct sockaddr *) &clie_data, (socklen_t*)&testInt);
        std::cout << "TRANSLATE MESSAGE RECEIVED\n";
        std::string reply(buffer);
        reply = reply.substr(1,reply.length());

        // It is here that I do some shenenigans

        try {
            // I'm just going to make a bunch of ifs to check lol
            reply = words.at(reply);
        } catch (const std::out_of_range& oor) {
            reply = "Word not found...";
        }

        std::cout << "TRANSLATE: " << reply << "\n";

        sendto(sock, reply.c_str(), reply.length(), MSG_CONFIRM, (const struct sockaddr *) &clie_data, sizeof(sockaddr_in));
    }

    return 0;
}

int convertFunc() {

    //std::cout << "\nConvert\n";

    // Map of conversions based on CAD
    std::unordered_map<std::string, float> conv =
        {{"CAD", 1},
        {"USD", 0.81},
        {"EUR", 0.7},
        {"BTC", 0.000016},
        {"GBP", 0.6}};

    int sock;
    struct sockaddr_in serv_data;
    struct sockaddr_in clie_data;
    struct sockaddr *client;

    sock = socket (AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cout << "Failed to create CONVERT socket\n";
        return 1; 
    }

    //Init memory
    memset(&serv_data, 0, sizeof(sockaddr_in));
    serv_data.sin_family = AF_INET;
    serv_data.sin_port = htons(convertPort);
    serv_data.sin_addr.s_addr = INADDR_ANY;

    //Give socket local address defined in server
    if (bind(sock,
             (const struct sockaddr *)&serv_data,
             sizeof(sockaddr_in)) < 0)
    {
        std::cout << "Failed to bind CONVERT socket\n";
        close(sock);
        return -1;
    }

    int readSize;
    int testInt = sizeof(sockaddr_in);
    char buffer[1024] = {0};

    // Runs indefinitely
    while (1) {

        float user = 0;

        std::string reply = "Convert's reply lol";

        readSize = recvfrom(sock, buffer, 1024, MSG_WAITALL, (struct sockaddr *) &clie_data, (socklen_t*)&testInt);
        std::string userNum (buffer);
        userNum = userNum.substr(1,userNum.length());
        std::cout << "CONVERT: " << userNum << "\n";
        
        readSize = recvfrom(sock, buffer, 1024, MSG_WAITALL, (struct sockaddr *) &clie_data, (socklen_t*)&testInt);
        std::string srcCur (buffer);
        srcCur = srcCur.substr(1,srcCur.length());
        std::cout << "CONVERT: " << srcCur << "\n";

        readSize = recvfrom(sock, buffer, 1024, MSG_WAITALL, (struct sockaddr *) &clie_data, (socklen_t*)&testInt);
        std::string destCur (buffer);
        destCur = destCur.substr(1,destCur.length());
        std::cout << "CONVERT: " << destCur << "\n";        

        // Converts the value into a float

        /* Put some code here */
        // Incase it fails to convert
        try {
            user = std::stof(userNum);
        } catch (const std::invalid_argument& ia) {
            user = 0;
            userNum = "";
            reply = "Failed to get value";
        }
        user = std::stof(userNum);

        if (userNum.length() < 1) {
            reply = "invalid value";
            sendto(sock, reply.c_str(), reply.length(), MSG_CONFIRM, (const struct sockaddr *) &clie_data, sizeof(sockaddr_in));
        } else {
            // Performs unit analysis to get desired currency
            std::cout << "Value used: " << user << "\n";

            /* Put more code here */
            try {
                // In case the currencies given aren't in the table
                user = user / conv.at(srcCur);
                user = user * conv.at(destCur);
                reply = "\nCurrency: ";
                reply += std::to_string(user);
            } catch (const std::out_of_range& oor) {
                reply = "Currency not found...";
            } 

            sendto(sock, reply.c_str(), reply.length(), MSG_CONFIRM, (const struct sockaddr *) &clie_data, sizeof(sockaddr_in));
        }
    }

    return 0;
}

int voteFunc() {

    //std::cout << "\nVote\n";

    // Who are the candidates?

    // 1. Joe
    // 2. Mike
    // 3. Candice
    // 4. Bofa

    // Imma just toss these into a switch case
    // and make them all ints. I've had enough I guess

    int joe = 290;
    int mike = 213;
    int candice = 216;
    int bofa = 123;

    int sock;
    struct sockaddr_in serv_data;
    struct sockaddr_in clie_data;
    struct sockaddr *client;

    std::string cand = "\t1. Joe\n\t2. Mike\n\t3. Candice\n\t4. Bofa\n";

    sock = socket (AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cout << "Failed to create VOTE socket\n";
        return 1; 
    }

    //Init memory
    memset(&serv_data, 0, sizeof(sockaddr_in));
    serv_data.sin_family = AF_INET;
    serv_data.sin_port = htons(votePort);
    serv_data.sin_addr.s_addr = INADDR_ANY;

    //Give socket local address defined in server
    if (bind(sock,
             (const struct sockaddr *)&serv_data,
             sizeof(sockaddr_in)) < 0)
    {
        std::cout << "Failed to bind VOTE socket\n";
        close(sock);
        return -1;
    }

    int readSize;
    char buffer[1024] = {0};
    int testInt = sizeof(sockaddr_in);
    client = (struct sockaddr *) &clie_data;

    // Runs indefinitely
    while (1) {
        int voted = 0;
        srand(time(NULL));
        int key = (rand() % 9) + 1; // Produces random number from 1 to 9. 0 would be a bad key
        int resp = 0;

        std::string reply;

        readSize = recvfrom(sock, buffer, 1024, MSG_WAITALL, client, (socklen_t*)&testInt);
        std::cout << "VOTE MESSAGE RECEIVED\n";
        std::cout << "VOTE: " << buffer << "\n";

        if (buffer[0] == 's') {
            std::cout << "Starting secure vote: " << buffer << "\nWith key: " << key << "\n";
            reply = std::to_string(key) + cand;
            //std::cout << "Vote sending:\n" << reply << "\nSize:" << reply.length() << "\n";
            
            // Why no send?
            sendto(sock, reply.c_str(), reply.length(), MSG_CONFIRM, (const struct sockaddr *) &clie_data, sizeof(sockaddr_in));
            //sendto(sock, "Bruh\n", 5, 0, (const struct sockaddr *) &clie_data, sizeof(sockaddr_in));
            //std::cout << "Starting vote shenanigans\n";
        
            // Gets the user's vote
            readSize = recvfrom(sock, buffer, 1024, MSG_WAITALL, client, (socklen_t*)&testInt);

            resp = buffer[0] - 48;
            resp /= key;

            std::cout << "User's vote: " << resp << "\n";
            // NOW I can work with the vote. Thank the lord it's almost over

            switch (resp) {
                case 1: joe++; voted = 1; break;
                case 2: mike++; voted = 1; break;
                case 3: candice++; voted = 1; break;
                case 4: bofa++; voted = 1; break;
                default: break;
            }

            // insert some shenanigans

            reply = "Thank you for voting, would you like to see results? (y,n):";
            sendto(sock, reply.c_str(), reply.length(), MSG_CONFIRM, (const struct sockaddr *) &clie_data, sizeof(sockaddr_in));

            // Gets the user's vote
            readSize = recvfrom(sock, buffer, 1024, MSG_WAITALL, client, (socklen_t*)&testInt);

            std::cout << "User choice: " << buffer << "\n";

            if (buffer[0] == 'y' && (voted == 1)) {
                reply = "";
                reply += "\nJoe: " + std::to_string(joe);
                reply += "\nMike: " + std::to_string(mike);
                reply += "\nCandice: " + std::to_string(candice);
                reply += "\nBofa: " + std::to_string(bofa);
                // insert some shenanigans
            } else {
                reply = "Thank you for your time";
            }
            sendto(sock, reply.c_str(), reply.length(), MSG_CONFIRM, (const struct sockaddr *) &clie_data, sizeof(sockaddr_in));

        } else {
            std::cout << "Man, how did I even get here?\n";
        }

    }

    return 0;
}

// Called by parent process to kill children processes
// BY DEFINITION, these threads usually are under 1 year old
void infanticide (kiddos* kids) {
    // Should probably send an exit message to the server before doing this
    // Or else their UDP sockets will take up a bunch of resources lols
    std::cout << "\nKilling children...\n";
    kill (kids -> tr, 1);
    kill (kids -> cv, 1);
    kill (kids -> vo, 1);
}

void handleConnect(int* EXIT_COND, kiddos* kids) {
    int server_fd, client_fd, size_of_smth = sizeof(sockaddr_in);
    int UDP_sock;

    // Addresses of all servers
    sockaddr_in serv_data;
    sockaddr_in translate_data;
    sockaddr_in convert_data;
    sockaddr_in vote_data;

    // Struct for microserver address
    struct sockaddr *client_t;
    struct sockaddr *client_c;
    struct sockaddr *client_v;

    // Setup of main TCP socket
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Failed to create server socket"
                  << "\n";
        infanticide(kids);
        *(EXIT_COND) = 0;
        return;
    }

    // Setup of UDP socket
    UDP_sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (UDP_sock == -1) {
        std::cout << "Failed to create MAIN socket\n";
        return; 
    }

    // Sets address for micro servers and main server
    memset(&serv_data, 0, sizeof(sockaddr_in));
    serv_data.sin_family = AF_INET;
    serv_data.sin_port = htons(mainPort);
    serv_data.sin_addr.s_addr = htonl(INADDR_ANY);

    memset(&translate_data, 0, sizeof(sockaddr_in));
    translate_data.sin_family = AF_INET;
    translate_data.sin_port = htons(translatePort);
    // Converts IP address to binary
    if (inet_pton(AF_INET, IPv4, &serv_data.sin_addr) <= 0) {
        std::cout << "\nTRANSLATE IP failed to convert\n";
    }

    memset(&convert_data, 0, sizeof(sockaddr_in));
    convert_data.sin_family = AF_INET;
    convert_data.sin_port = htons(convertPort);
    // Converts IP address to binary
    if (inet_pton(AF_INET, IPv4, &serv_data.sin_addr) <= 0) {
        std::cout << "\nCONVERT IP failed to convert\n";
    }

    memset(&vote_data, 0, sizeof(sockaddr_in));
    vote_data.sin_family = AF_INET;
    vote_data.sin_port = htons(votePort);
    // Converts IP address to binary
    if (inet_pton(AF_INET, IPv4, &serv_data.sin_addr) <= 0) {
        std::cout << "\nVOTE IP failed to convert\n";
    }

    client_t = (struct sockaddr *) &translate_data;
    client_c = (struct sockaddr *) &convert_data;
    client_v = (struct sockaddr *) &vote_data;
    
    //Give socket local address defined in server
    if (bind(server_fd,
             (const struct sockaddr *)&serv_data,
             sizeof(sockaddr_in)) < 0)
    {
        std::cout << "Failed to bind server socket\n";
        close(server_fd);
        return;
    }

    //Prepare socket to accept incoming tcp connections
    if (listen(server_fd, 2) < 0)
    {
        std::cout << "Couldn't listen\n";
        close(server_fd);
        infanticide(kids);
        *(EXIT_COND) = 0;
        return;
    }

    // Timeout code found online incase something happened to the packets
    // and the connection need to timeout
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    if (setsockopt(UDP_sock, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        std::cout << "opt error \n";
        return;
    }

    std::cout << "Server is running\n";
    std::cout << "on port: " << mainPort << "\n";
    std::cout << "on IP: " << IPv4 << "\n";

    // While loop that runs the server
    while (*(EXIT_COND))
    {
        // Waits for client side request
        // This means if at any point running becomes false, it must wait for the last request
        // before terminating the program
        if ((client_fd = accept(server_fd,
                                (struct sockaddr *)&serv_data,
                                (socklen_t *)&size_of_smth)) < 0)
        {
            // Accept failed, program terminates
            std::cout << "Closing\n";
            close(client_fd);
            *(EXIT_COND) = 0;
            return;
        }
        else if (*(EXIT_COND))
        {
            // Accept succeeded, handles request
            char buffer[1024] = {0};
            int readSize = 0;

            // Probing UDP servers if they are still alive 🤣
            int size_send = 0;
            // sendto(UDP_sock, message.c_str(), message.length(), MSG_CONFIRM, client_t, sizeof(sockaddr_in));
            // sendto(UDP_sock, message.c_str(), message.length(), MSG_CONFIRM, client_c, sizeof(sockaddr_in));
            // sendto(UDP_sock, message.c_str(), message.length(), MSG_CONFIRM, client_v, sizeof(sockaddr_in));

            // This is where I handle the connection

            std::string message (buffer);
            std::string reply = "...";
            int testint = sizeof(sockaddr_in);

            do {
                readSize = recv(client_fd, buffer, 1024, 0);

                // Time for the most disgusting means of receiving messages
                std::string msg (buffer);   // Create string
                message.assign(msg);              // Make it equal to old message LOL

                //std::cout << "\nmessages read\n";

                // Decides the action based on the user's request
                if (message[1] == '1') {        // Translation
                    std::cout << "Translate state START\n";
                    send (client_fd, "t", 1, 0);


                    // Waits for user's input
                    readSize = recv(client_fd, buffer, 1024, 0);
                    
                    // Send to the UDP server and waits for a response
                    sendto(UDP_sock, buffer, 1024, MSG_CONFIRM, client_t, sizeof(sockaddr_in));
                    
                    // Flush buffer after each use
                    for (int i = 0; i < 1024; i++) buffer[i] = 0;
                    
                    readSize = recvfrom(UDP_sock, buffer, 1024, MSG_WAITALL, client_t, (socklen_t*)&testint);

                    std::cout << "Translate returned: " << buffer << "\n";
 
                    send (client_fd, buffer, 1024, 0);

                    std::cout << "Translate state END\n";

                } else if (message[1] == '2') { // Convert
                    std::cout << "Convert state START\n";
                    send (client_fd, "c", 1, 0);

                    // Gets Value from client and sends to microserver
                    readSize = recv(client_fd, buffer, 1024, 0);
                    send (client_fd, reply.c_str(), reply.length(), 0);
                    std::cout << "Value: " << buffer << "\n";
                    sendto(UDP_sock, buffer, 1024, MSG_CONFIRM, client_c, sizeof(sockaddr_in));
                    for (int i = 0; i < 1024; i++) buffer[i] = 0;

                    // Gets Source Currency from client and sends to microserver
                    readSize = recv(client_fd, buffer, 1024, 0);
                    send (client_fd, reply.c_str(), reply.length(), 0);
                    std::cout << "Source: " << buffer << "\n";
                    sendto(UDP_sock, buffer, 1024, MSG_CONFIRM, client_c, sizeof(sockaddr_in));
                    for (int i = 0; i < 1024; i++) buffer[i] = 0;

                    // Gets Destination Currency from client and sends to microserver
                    readSize = recv(client_fd, buffer, 1024, 0);
                    std::cout << "Destination: " << buffer << "\n";
                    sendto(UDP_sock, buffer, 1024, MSG_CONFIRM, client_c, sizeof(sockaddr_in));

                    readSize = recvfrom(UDP_sock, buffer, 1024, MSG_WAITALL, client_c, (socklen_t*)&testint);
                    std::cout << "Convert returned: " << buffer << "\n";

                    send (client_fd, buffer, 1024, 0);

                    std::cout << "Convert state END\n";

                } else if (message[1] == '3') { // Vote
                    std::cout << "Vote state START\n";
                    send (client_fd, "v", 1, 0);

                    // Retrieves candidate list
                    readSize = recv(client_fd, buffer, 1024, 0);
                    //for (int i = 0; i < 1024; i++) buffer[i] = 0;
                    // Prompts UDP for start
                    sendto(UDP_sock, "start", 1024, MSG_CONFIRM, client_v, sizeof(sockaddr_in));
                    readSize = recvfrom(UDP_sock, buffer, 1024, MSG_WAITALL, client_v, (socklen_t*)&testint);
                    //std::cout << "VOTE SENT: " << buffer << "\n";
                    send (client_fd, buffer, 1024, 0);

                    for (int i = 0; i < 1024; i++) buffer[i] = 0;

                    // Sends user's response to server
                    readSize = recv(client_fd, buffer, 1024, 0);
                    sendto(UDP_sock, buffer, 1024, MSG_CONFIRM, client_v, sizeof(sockaddr_in));
                    readSize = recvfrom(UDP_sock, buffer, 1024, MSG_WAITALL, client_v, (socklen_t*)&testint);
                    send (client_fd, buffer, 1024, 0);

                    for (int i = 0; i < 1024; i++) buffer[i] = 0;

                    // Checks if they want results, and sends if they do                    
                    readSize = recv(client_fd, buffer, 1024, 0);
                    sendto(UDP_sock, buffer, 1024, MSG_CONFIRM, client_v, sizeof(sockaddr_in));
                    readSize = recvfrom(UDP_sock, buffer, 1024, MSG_WAITALL, client_v, (socklen_t*)&testint);
                    send (client_fd, buffer, 1024, 0);

                    std::cout << "Vote state END\n";

                } else {
                    if (message != "c:EXIT") std::cout << "User invalid request\n";
                    send (client_fd, "f", 1, 0);
                }

                // Flush buffer after each use
                for (int i = 0; i < 1024; i++) buffer[i] = 0;

            } while (message != "c:EXIT");

            // std::cout << "Servers probed\n";

            close(client_fd);
        }
    }
}

int mainServ(kiddos* kids) {

    int EXIT_COND = 1;

    std::cout << "\nProcesses created with the following IDs:\n";
    std::cout << kids -> tr << "\n" << kids -> cv << "\n" << kids -> vo << "\n";

    // Creates connection thread
    // Decided to switch how it was compared to previous assignment
    // where the connections are handled in a thread and server
    // terminal is the main thread
    std::thread th1(handleConnect, &EXIT_COND, kids);

    std::string userIn;
    do {
        std::cout << "server side shenanigans -$ ";
        std::cin >> userIn;
        std::cout << "Testing input: " << userIn << "\n";

        if (!EXIT_COND) {
            std::cout << "Something went wrong...\n";
            userIn = ":EXIT";
        }
    } while (userIn != ":EXIT");

    EXIT_COND = 0;

    infanticide(kids);
    th1.join();
    return 0;
}

int main () {

    // This main function just creates a bunch of processes
    // and pushes all other work to functions.
    // Kinda sucks that it's all in this single file tho
    // RIP .text for each process

    pid_t translate;
    pid_t convert;
    pid_t vote;

    // So begins the forking lols
    translate = fork();
    if (translate == 0) {
        translateFunc();
    } else if (translate > 0) {
        convert = fork();
        if (convert == 0) {
            convertFunc();
        } else if (convert > 0) {
            vote = fork();
            if (vote == 0) {
                voteFunc();
            } else if (vote > 0) {

                kiddos kids = {translate,convert,vote};

                mainServ(&kids);
            } else {
                std::cout << "\nFailed to create vote process\n";
            }
        } else {
            std::cout << "\nFailed to create convert process\n";
        }
    } else {
        std::cout << "\nFailed to create translate process\n";
    }

    return 0;
}