#pragma once

extern "C"
{
    #include "lwip/udp.h"
}


namespace lwip
{
    class Udp
    {
        struct udp_pcb* pcb;

    public:
        Udp() { pcb = udp_new(); }
        ~Udp() { udp_remove(pcb); }

        err_t bind(struct ip_addr* ipaddr, u16_t port)
        {
            return udp_bind(pcb, ipaddr, port);
        }

        // connected only receives data from the connected address,
        // unconnected receive from any address
        err_t connect(struct ip_addr* ipaddr, u16_t port)
        {
            return udp_connect(pcb, ipaddr, port);
        }

        void disconnect() { udp_disconnect(pcb); }

        // Remember, the callback is responsible for deallocating the pbuf
        void recv(void (*recv_f)(void * arg,
            struct udp_pcb* upcb,
            struct pbuf* p,
            struct ip_addr* addr,
            u16_t port), void* recv_arg)
        {
            udp_recv(pcb, recv_f, recv_arg);
        }
    };
}
