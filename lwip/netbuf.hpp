#pragma once

extern "C"
{
    #include "lwip/netbuf.h"
}


namespace lwip
{
    class Netconn;

    class Netbuf
    {
        netbuf* buf;

        friend Netconn;

    protected:
        void setBuffer(netbuf* buf) { this->buf = buf; }

    public:
        Netbuf(bool autoAllocate = true)
        {
            if(autoAllocate)
                buf = netbuf_new();
        }

        // auto-calls ref for this one. be careful, because
        // you can't get your err_t back
        Netbuf(const void* data, u16_t size)
        {
            buf = netbuf_new();
            ref(data, size);
        }

        /*
        Netbuf(size_t size)
        {
            buf = netbuf_new();
            alloc(size);
        }*/

        Netbuf(netbuf* buf) { this->buf = buf; }

        operator netbuf*() { return buf; }

        size_t len() { return netbuf_len(buf); }

        // retrieve part of the chained buffer inside
        err_t data(void** dataptr, u16_t* len)
        {
            return netbuf_data(buf, dataptr, len);
        }

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
