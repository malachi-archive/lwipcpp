#pragma once

extern "C"
{
    #include "lwip/netbuf.h"
}


namespace lwip
{
    class Netbuf
    {
        netbuf* buf;

    public:
        //Netbuf() { newbuf_new(&buf); }

        /*
        Netbuf(size_t size)
        {
            buf = netbuf_new();
            alloc(size);
        }*/

        Netbuf() { buf = netbuf_new(); }

        Netbuf(netbuf* buf) { this->buf = buf; }

        operator netbuf*() { return buf; }

        size_t len() { return netbuf_len(buf); }

        void* alloc(size_t size)
        {
            return netbuf_alloc(buf, size);
        }

        err_t ref(const void* data, u16_t size)
        {
            return netbuf_ref(buf, data, size);
        }

        void del() { return netbuf_delete(buf); }

        err_t copy(void* dest, u16_t size)
        {
            return netbuf_copy(buf, dest, size);
        }

        ip_addr_t* fromAddr()
        {
            return netbuf_fromaddr(buf);
        }

        u16_t fromPort()
        {
            return netbuf_fromport(buf);
        }
    };
}
