#pragma once

extern "C"
{
    #include "lwip/pbuf.h"
}


namespace lwip
{
    class PacketBuffer
    {
        struct pbuf* p;

    public:
        PacketBuffer(struct pbuf* p)
        {
            this->p = p;
        }

        operator pbuf*() const { return p; }

        u16_t copyPartial(void * dataptr, u16_t len, u16_t offset) const
        {
            return pbuf_copy_partial(p, dataptr, len, offset);
        }

        void ref() { pbuf_ref(p); }
        u8_t free() { return pbuf_free(p); }

        u8_t getAt(u16_t offset) { return pbuf_get_at(p, offset); }
    };
}
