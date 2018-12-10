#include <sys/types.h>  /* for type definitions */
#include <sys/socket.h> /* for socket API function calls */
#include <netinet/in.h> /* for address structs */
#include <arpa/inet.h>  /* for sockaddr_in */
#include <stdio.h>      /* for printf() */
#include <stdlib.h>     /* for atoi() */
#include <string.h>     /* for strlen() */
#include <unistd.h>     /* for close() */

class Multicast
{
  private:
    /* data */
    const int MIN_PORT = 1024;
    const int MAX_PORT = 65535;
    const int MAX_BUFFER_SIZE = 256;
    char *ip_group;
    int port, addrlen, nbytes;
    struct sockaddr_in address;
    struct ip_mreq mreq;
    int socketNumber;
    char buffer_data[256];

  public:
    Multicast(char *ip, int port);
    ~Multicast();
    int sendMessage(char *message);
    char *receiveMessage();
};