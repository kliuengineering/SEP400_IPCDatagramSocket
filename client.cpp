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
const int PORT_SERVER = 5000;
const int SIZE = 30;
char buffer[] = "this is a message, be careful!";
char buffer_receive[SIZE];

void CreateSocket(int &fd)
{
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        std::cerr << "CreateSocket() failed...\n";
        exit(EXIT_FAILURE);     
    }

    std::cout << "CLIENT: socket created successfully...\n";
}

void SetAddressDestination(sockaddr_in &addr_serv, int &rc, int &fd)
{
    memset( (char*)&addr_serv, 0, sizeof(addr_serv) );
    addr_serv.sin_family = AF_INET;
    
    rc = inet_pton(AF_INET, ADDR_SERVER, &addr_serv.sin_addr);
    if(rc < 0)
    {
        std::cerr << "SetAddressDestination() failed...\n";
        close(fd);
        exit(EXIT_FAILURE);           
    }

    addr_serv.sin_port = htons(PORT_SERVER);

    std::cout << "CLIENT: destination socket address set successfully...\n";
}

void Send(struct sockaddr_in &addr_serv, socklen_t addr_len, int &length, int &rc, int &fd)
{
    length = strlen(buffer) + 1;

    rc = sendto(fd, buffer, length, 0, (struct sockaddr *)&addr_serv, addr_len);
    if (rc < 0)
    {
        std::cerr << "Send() failed...\n";
        close(fd);
        exit(EXIT_FAILURE);             
    }

    std::cout << "CLIENT: send successfully...\n";
}

void Receive(struct sockaddr_in &addr_sender, socklen_t addr_len, int &length, int &rc, int &fd)
{
    memset(buffer_receive, 0, SIZE);

    rc = recvfrom( fd, buffer_receive, length, 0, (struct sockaddr *)&addr_sender, &addr_len );
    if(rc < 0)
    {
        std::cerr << "Receive() failed...\n";
        close(fd);
        exit(EXIT_FAILURE);            
    }

    std::cout << "CLIENT: received successfully...\n";
}

int main(int argc, char *argv[])
{
    // variable declaration
    int fd;
    int rc;
    int length;

    struct sockaddr_in addr_serv;
    struct sockaddr_in addr_sender;
    socklen_t addr_len = sizeof(addr_serv);

    CreateSocket(fd);
    SetAddressDestination(addr_serv, rc, fd);
    Send(addr_serv, addr_len, length, rc, fd);
    Receive(addr_sender, addr_len, length, rc, fd);

    std::cout << "CLIENT: received -> " << buffer_receive << std::endl;

    return EXIT_SUCCESS;
}
