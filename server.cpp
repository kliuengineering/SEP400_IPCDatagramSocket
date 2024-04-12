#include <arpa/inet.h>
#include <iostream>
#include <net/if.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

const char ADDR_SERVER[] = "10.211.55.5";
const char PORT_SERVER = 5000;
const int SIZE = 30;

void CreateSocket(int &fd)
{
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        std::cerr << "CreateSocket() failed...\n";
        exit(EXIT_FAILURE);
    }
}

void SetAddress(struct sockaddr_in &addr_serv, int &rc, int &fd)
{
    memset( (char *)&addr_serv, 0, sizeof(addr_serv) );
    addr_serv.sin_family = AF_INET;
    
    rc = inet_pton(AF_INET, ADDR_SERVER, &addr_serv.sin_addr);
    if(rc < 0)
    {
        std::cerr << "SetAddress() failed...\n";
        exit(EXIT_FAILURE);       
    }
    addr_serv.sin_port = htons(PORT_SERVER);
}

void BindSocket(struct sockaddr_in &addr_serv, int &rc, int &fd)
{
    rc = bind( fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv) );
    if(rc < 0)
    {
        std::cerr << "BindSocket() failed...\n";
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void Receive(char buffer[], sockaddr_in &addr_cli, socklen_t &addr_len, int &length, int &fd)
{
    length = recvfrom(fd, buffer, SIZE, 0, (struct sockaddr *)&addr_cli, &addr_len);
    if( length < 0)
    {
        std::cerr << "Receive() failed...\n";
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void Send(char old_buffer[], struct sockaddr_in &addr_cli, socklen_t &addr_len, int &length, int &fd)
{
    char new_buffer[SIZE];
    memset(new_buffer, 0, sizeof(new_buffer));

    for(int i = 0; i < length, i++)
    {
        new_buffer[i] = toupper(old_buffer[i]);
    }

    length = sendto( fd, new_buffer, length, 0, (struct sockaddr *)&addr_cli, &addr_len );
    if (length < 0)
    {
        std::cerr << "Send() failed...\n";
        close(fd);
        exit(EXIT_FAILURE);        
    }
}

int main(int argc, char *argv[])
{
    int fd;
    int rc;
    int length;

    // need to declare address info first
    struct sockaddr_in addr_serv;
    struct sockaddr_in addr_cli;
    socklen_t addr_len = sizeof(addr_cli);
    char buffer[SIZE];

    CreateSocket(fd);
    SetAddress(addr_serv, rc, fd);
    BindSocket(addr_serv, rc, fd);
    Receive(buffer, addr_cli, addr_len, length, fd);
        std::cout << "SERVER: Received -> " << buffer << std::endl;
    Send(buffer, addr_cli, addr_len, length, fd);


    
    
    close(fd);
    return EXIT_SUCCESS;
}