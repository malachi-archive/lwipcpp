#pragma once

extern "C"
{
    #include "lwip/netbuf.h"
}

#ifndef ASSERT
#define ASSERT(condition, message)
#endif


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
            
            ASSERT(buf != NULL, "Buf cannot be NULL");
            
            ref(data, size);
        }

        /*
        Netbuf(size_t size)
        {
            buf = netbuf_new();
            alloc(size);
        }*/

        Netbuf(netbuf* buf) { this->buf = buf; }

        operator netbuf*() const { return buf; }

        size_t len() const { return netbuf_len(buf); }

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
            err_t result = netbuf_ref(buf, data, size);
            
            ASSERT(result, "failed with code: " << result);
            
            return result;
        }

        void del() { return netbuf_delete(buf); }
        
        void free() { return netbuf_free(buf); }

        err_t copy(void* dest, u16_t size)
        {
            return netbuf_copy(buf, dest, size);
        }

        ip_addr_t* fromAddr() const
        {
            return netbuf_fromaddr(buf);
        }

        u16_t fromPort() const
        {
            return netbuf_fromport(buf);
        }
    };
}
