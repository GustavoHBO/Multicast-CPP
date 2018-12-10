#include "./include/Multicast.hpp"

Multicast::Multicast(char *ip, int port)
{
    if (port < MIN_PORT || port > MAX_PORT)
    {
        perror("Port invalid!");
    }
    else  
    {
        ip_group = ip;
        this->port = port;

        /* create what looks like an ordinary UDP socket */
        if ((socketNumber = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket");
        }

        /* set up destination address */
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(ip_group);
        address.sin_port = htons(port);
        u_int len = 1;
        if (setsockopt(socketNumber, SOL_SOCKET, SO_REUSEADDR, &len, sizeof(len)) < 0)
        {
            perror("Reusing ADDR failed");
        }
        /* bind to receive address */
        if (bind(socketNumber, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("bind");
        }
        /* use setsockopt() to request that the kernel join a multicast group */
        mreq.imr_multiaddr.s_addr = inet_addr(ip_group);
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(socketNumber, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
        {
            perror("setsockopt");
        }
    }
}

Multicast::~Multicast()
{
    close(socketNumber);
}

int Multicast::sendMessage(char *msg)
{
    if (sendto(socketNumber, msg, strlen(msg), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("sendto");
        return -1;
    }
}

char *Multicast::receiveMessage()
{
    addrlen = sizeof(address);
    if ((nbytes = recvfrom(socketNumber, buffer_data, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("recvfrom");
        return NULL;
    }
    if (nbytes > 0)
    {
        return buffer_data;
    }
}