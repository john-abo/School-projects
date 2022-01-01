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
#include <algorithm>
#include <stdio.h> //fprintf

typedef struct sockaddr_in sockaddr_in;

bool running = true;
int iTrue = 1;

int serv_listen(std::vector<std::string> *bl, int sock);
int endCondition();
int readDelim(int fileFD, std::string &buff, const char delim, int count);
int handleRequest(int sock, std::vector<std::string> &blacklist);
int urlToken(std::string &url, std::string &host, std::string &addr);

int main()
{
    std::vector<std::string> blacklist;

    int inPORT = 6901;
    int server_fd, client_fd, size_of_smth = sizeof(sockaddr_in);
    sockaddr_in serv_data;

    std::cout << "Enter port #: \n";
    std::cin >> inPORT;
    std::cout << "Listening on port: " << inPORT << "\n";

    //Init memory
    memset(&serv_data, 0, sizeof(sockaddr_in));
    serv_data.sin_family = AF_INET;
    serv_data.sin_port = htons(inPORT);
    serv_data.sin_addr.s_addr = htonl(INADDR_ANY);

    // Following code was borrowed from

    //Create IPv4 Protocol Family using a TCP transport, default protocols
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Failed to create server socket"
                  << "\n";
        return 1;
    }
    //std::cout << "Created server socket\n";

    //Give socket local address defined in server
    if (bind(server_fd,
             (const struct sockaddr *)&serv_data,
             sizeof(sockaddr_in)) < 0)
    {
        std::cout << "Failed to bind server socket\n";
        close(server_fd);
        return -1;
    }
    //std::cout << "Bound server socket\n";

    //Prepare socket to accept incoming tcp connections
    if (listen(server_fd, 2) < 0)
    {
        std::cout << "Couldn't listen\n";
        close(server_fd);
        return 1;
    }
    //std::cout << "Prepared socket to accept TCP connections\n";

    // Opens blacklist thread if proxy server is successfully setup
    std::thread server_side_listen(serv_listen, &blacklist, server_fd);

    // Infinite loop, waiting for response
    while (running)
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
            return 1;
        }
        else if (running)
        {
            // Accept succeeded, handles request
            // this is where the fun begins... ====================================================

            // I could just pass this into a thread so I can handle several requests as once lmao
            handleRequest(client_fd, blacklist);

            // ====================================================================================
        }
    }

    // Closes any potential requests
    close(server_fd);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &iTrue, sizeof(int));

    // Closes thread of blacklist listening
    server_side_listen.join();

    return 0;
}

// reads from file descriptor and writes to string until delimiter
int readDelim(int fileFD, std::string &buff, const char delim, int count)
{
    int i = -1;
    char charIn[1] = {0};

    do
    {
        read(fileFD, charIn, 1);
        buff.push_back(charIn[0]);
        i++;

        if ((*charIn) == delim)
            count--;
    } while ((*charIn) != delim || count > 0);

    return i;
}

// Checks and updates the blacklist with server side commands
int serv_listen(std::vector<std::string> *bl, int sock)
{
    std::string in;
    //std::cout << "Enter blacklist word: \n";

    while (running)
    {
        std::cout << "ProxyTerminal::& ";
        std::cin >> in;

        if (in[0] == ':' && in[1] == ':')
        {
            //std::cout << "in was recognized as a command:\n"
            //          << in << "\n";

            // Included some command for server side control
            if (in == "::QUIT") // Ends program
            {
                running = false;
                std::cout << "\nQuitting\n";
            }

            if (in == "::FUN") // Little easter egg
                std::cout << "CPSC 441 is not fun...\n";

            if (in == "::PRINT") // Prints content of blacklist
            {
                std::cout << "Printing blacklist...\n";

                for (std::string s : (*bl))
                {
                    std::cout << s << "\n";
                }
            }

            if (in == "::REMOVE")
            {
                std::string target;
                std::cin >> target;

                std::cout << "Removing " << target <<  "...\n";

                (*bl).erase(std::remove((*bl).begin(), (*bl).end(), target), (*bl).end());
            }
        }
        else
        {
            std::cout << "Adding: " << in << " to blacklist\n";
            (*bl).push_back(in);
        }
    }

    return 0;
}

int handleRequest(int sock, std::vector<std::string> &blacklist)
{
    // A lot of strings, probably not very memory efficient
    std::string _HOST;
    std::string type = "";
    std::string url = "";
    std::string host = "";
    std::string addr = "";
    std::string buffString = "";

    // Other variables used in the function
    int destSock, response, success = 0;
    int time;
    
    bool banned = false;
    bool undertime = true;
    char buffer[8192] = {0};
    char pipe[1] = {0};

    std::size_t strFound;

    // Address of destination
    sockaddr_in dest_addr = {0};
    struct hostent *address;

    // Gets request type, URL, and protocol used
    readDelim(sock, type, ' ', 1);
    readDelim(sock, url, ' ', 1);
    readDelim(sock, buffString, '\n', 1); // this reads the request protocol and adds to buffer

    // Reads remaining buffer, converts to string
    read(sock, buffer, 8192);
    buffString = (buffer);

    // Here I started tokenizing the string and actually being a proxy
    int test = urlToken(url, host, addr);

    // Checks URL if any word in blacklist exists in string
    for (std::string &str : blacklist)
    {
        std::cout << "List: " << str << "\n";
        strFound = url.find(str);
        if (strFound != std::string::npos)
        {
            std::cout << "Found: " << str << "\n";
            banned = true;
        }
    }

    if (banned) // The user tried to access forbidden content
    {
        //std::cout << "Banned word...\n";
        _HOST = "pages.cpsc.ucalgary.ca";
        addr = "/~carey/CPSC441/ass1/error.html ";  // I forgot a space
    }
    else
    {
        // Start getting data from server side, default is going to need to be the error page
        //std::cout << "Valid word...\n";
        _HOST = host;
    }

    // gets address of _HOST, begins connecting to server
    address = gethostbyname(_HOST.c_str());
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(80);

    // Creates socket
    if ((destSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        std::cout << "\nFailed to create socket\n";

    // Sets a timer for the socket to remain open
    struct timeval rec;
    rec.tv_sec = 3;
    if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,(struct timevacdl *)&rec,sizeof(struct timeval) )<0){
        std::cout << "opt error ";
        close(sock);
        close(destSock);

        return 1;
    }

    if(setsockopt(destSock, SOL_SOCKET, SO_RCVTIMEO,(struct timevacdl *)&rec,sizeof(struct timeval) )<0){
        std::cout << "opt error ";
        close(sock);
        close(destSock);

        return 1;
    }

    // Converts IP address to binary
    inet_pton(AF_INET, address->h_addr, &dest_addr.sin_addr);
    bcopy((char *)address->h_addr, (char *)&dest_addr.sin_addr.s_addr, address->h_length);

    // Attempts to connect to the server
    connect(destSock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

    // Sends request to server, immediately pipes message to client
    std::string msg = "GET http://" + host + addr + "HTTP/1.1\r\n" + buffString;
    success = send(destSock, msg.c_str(), msg.length(), 0);
    std::cout << "\nREQUEST:\n" << msg << "\n";

    // While still receiving and sending requests, keep doing so 
    while (success > 0 && response > 0)  {

        response = recv(destSock, buffer, 8192, 0);
        success = send(sock, buffer, response, 0);
        for (int i = 0; i < 8192; i++)
            buffer[i] = 0;
        std::cout << "(" << response << "," << success << ")\n";
    }

    std::cout << "\nEnding data transfer\n";

    close(sock);
    close(destSock);

    return 0;
}

// Attempts to split url into host and address
int urlToken(std::string &url, std::string &host, std::string &addr)
{
    std::string del = "https://";
    std::size_t delimiter;
    url = url.substr(del.length() - 1, url.length());
    delimiter = url.find("/");
    addr = url.substr(delimiter);
    host = url.substr(0, url.length() - addr.length());
    return 0;
}