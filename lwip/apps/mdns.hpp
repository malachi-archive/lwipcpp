//#define DEBUG_MDNS_INITIALIZER

#include "mdns_opts.hpp"

extern "C" {
#include <lwip/netif.h>
#include <lwip/apps/mdns.h>
#include <lwip/igmp.h>

#if MDNS_SMART_INIT
#include <lwip/udp.h>
#endif

#ifdef DEBUG_MDNS_INITIALIZER
#include <FreeRTOS.h>
#endif

}


namespace lwip {

namespace mdns {

static void srv_txt(struct mdns_service *service, void *txt_userdata)
{
    mdns_resp_add_service_txtitem(service, "path=/", 6);
}


struct service_def
{
    const char* name;
    const char* service;
    enum mdns_sd_proto proto;
    uint16_t port;
    uint32_t ttl_seconds;
    service_get_txt_fn_t txt_fn;
    void* txt_data;
};

class Responder
{
    struct netif* n;

#ifdef MDNS_STATIC_CONSTRUCTOR
    struct static_constructor
    {
        static_constructor()
        {
            mdns_resp_init();
        }
    };

    static static_constructor _sc;
#endif
    static constexpr uint32_t ttl_seconds_default = 60;

#if MDNS_SMART_INIT
    // Attempts to rebind to what should be an already-bound
    // listener if mdns_resp_init was already called
    static bool detect_initialized()
    {
        auto mdns_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);

        //printf("MDNS: pcb = %p\r\n", mdns_pcb);

        LWIP_ASSERT("Failed to allocate pcb", mdns_pcb != NULL);;
        if(mdns_pcb == nullptr) return true;

        auto res = udp_bind(mdns_pcb, IP_ANY_TYPE, 5353);

        // Couldn't bind means already initialized
        if(res != ERR_OK)
            LWIP_DEBUGF(MDNS_DEBUG, ("MDNS: Could not bind udp - MDNS already initialized\n"));

        // Free up allocations
        udp_disconnect(mdns_pcb);
        udp_remove(mdns_pcb);

        // returns true when already initialized
        return res != ERR_OK;
    }
#endif

public:
    inline static void init()
    {
#if MDNS_SMART_INIT
#ifdef DEBUG_MDNS_INITIALIZER
        printf("Free heap 0: %lu\r\n", xPortGetFreeHeapSize());
#endif
        if(detect_initialized()) return;

#ifdef DEBUG_MDNS_INITIALIZER
        printf("Free heap 1: %lu\r\n", xPortGetFreeHeapSize());
#endif
#endif
        mdns_resp_init();
    }

    inline Responder(netif* n, const char* hostname, uint32_t ttl_seconds = ttl_seconds_default) : n(n)
    {
#if MDNS_SMART_INIT
        init();
#endif
        n->flags |= NETIF_FLAG_IGMP;
        mdns_resp_add_netif(n, hostname, ttl_seconds);
    }


    Responder(const char* name, const char* hostname, uint32_t ttl_seconds = ttl_seconds_default) :
        Responder(netif_find(name), hostname, ttl_seconds)
    {
    }

    Responder(const char* hostname, uint32_t ttl_seconds = ttl_seconds_default) :
        Responder(netif_default, hostname, ttl_seconds)
    {}


#if LWIP_NETIF_HOSTNAME
    Responder(netif* n, uint32_t ttl_seconds = ttl_seconds_default) :
        Responder(n, netif_get_hostname(n), ttl_seconds)
    {
    }

    Responder(uint32_t ttl_seconds = ttl_seconds_default) :
        Responder(netif_default, ttl_seconds)
    {
    }
#endif

    ~Responder() { mdns_resp_remove_netif(n); }

    inline err_t add_service(const char* name, const char* service, enum mdns_sd_proto proto,
        uint16_t port, uint32_t ttl_seconds, service_get_txt_fn_t txt_fn, void* txt_data)
    {
        return mdns_resp_add_service(n, name, service, proto, port,
            ttl_seconds, txt_fn, txt_data);
    }


    inline err_t add_service(const char* name, const char* service, enum mdns_sd_proto proto,
        uint16_t port, uint32_t ttl_seconds, service_get_txt_fn_t txt_fn)
    {
        return add_service(name, service, proto, port, ttl_seconds, txt_fn, this);
    }


    inline Responder& operator+=(Responder& (*__pf)(Responder&))
    {
        return __pf(*this);
    }

    inline Responder& operator+=(const service_def&& sd)
    {
        add_service(sd.name, sd.service, sd.proto, sd.port,
            sd.ttl_seconds, sd.txt_fn, sd.txt_data);
        return *this;
    }
};



inline const service_def https(const char* name = "secure-web", uint16_t port = 443, uint32_t ttl_seconds = 3600)
{
    service_def sd;

    sd.name = name;
    sd.service = "_https";
    sd.proto = DNSSD_PROTO_TCP;
    sd.port = port;
    sd.ttl_seconds = ttl_seconds;
    sd.txt_fn = srv_txt;
    sd.txt_data = nullptr;

    return sd;
}

inline const service_def http(const char* name = "web", uint16_t port = 80, uint32_t ttl_seconds = 3600)
{
    service_def sd;

    sd.name = name;
    sd.service = "_http";
    sd.proto = DNSSD_PROTO_TCP;
    sd.port = port;
    sd.ttl_seconds = ttl_seconds;
    sd.txt_fn = srv_txt;
    sd.txt_data = nullptr;

    return sd;
}

inline Responder& http(Responder& mdns)
{
    return mdns += http();
}

}


}
