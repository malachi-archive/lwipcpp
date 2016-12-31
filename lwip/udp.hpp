#pragma once

extern "C"
{
    #include "lwip/udp.h"
    #include "pbuf.hpp"
}


namespace lwip
{
    class UDP
    {
        struct udp_pcb* pcb;

    public:
        UDP() { pcb = udp_new(); }
        ~UDP() { udp_remove(pcb); }

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

        err_t send(struct pbuf* p)
        {
            return udp_send(pcb, p);
        }
    };

namespace layer5
{
    class UDP : public lwip::UDP
    {
    public:
        UDP()
        {
            recv([](
                void* arg, struct udp_pcb* upcb,
                struct pbuf* p,
                struct ip_addr* addr,
                u16_t port)
                {
                    ((UDP*)arg)->onReceive(PacketBuffer(p), addr, port);
                }, this);
        }

        virtual void onReceive(PacketBuffer p, const ip_addr_t *addr, u16_t port) = 0;
    };
}
}
