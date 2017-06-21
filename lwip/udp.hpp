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

        // NOTE: Old way, may still need this.  (Didn't actually ever
        // use typedef, instead sprinkled struct ip_addr around)
        //typedef struct ip_addr ip_addr_t;

    public:
        typedef ::ip_addr_t ip_addr_t;

        UDP() { pcb = udp_new(); }
        ~UDP() { udp_remove(pcb); }

        err_t bind(ip_addr_t* ipaddr, u16_t port)
        {
            return udp_bind(pcb, ipaddr, port);
        }

        // connected only receives data from the connected address,
        // unconnected receive from any address
        err_t connect(ip_addr_t* ipaddr, u16_t port)
        {
            return udp_connect(pcb, ipaddr, port);
        }

        void disconnect() { udp_disconnect(pcb); }

        /*
        // Remember, the callback is responsible for deallocating the pbuf
        void recv(void (*recv_f)(void * arg,
            struct udp_pcb* upcb,
            struct pbuf* p,
            struct ip4_addr* addr,
            u16_t port), void* recv_arg)
        {
            udp_recv(pcb, recv_f, recv_arg);
        } */

        void recv(udp_recv_fn recv_f, void* recv_arg)
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
                const ip_addr_t* addr,
                u16_t port)
                {
                    ((UDP*)arg)->onReceive(PacketBuffer(p), addr, port);
                }, this);
        }

        virtual void onReceive(PacketBuffer p, const ip_addr_t *addr, u16_t port) = 0;
    };
}
}
