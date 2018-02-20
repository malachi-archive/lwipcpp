extern "C"
{
    #include "lwip/opt.h"
}

#if LWIP_MDNS_RESPONDER

#include "mdns.hpp"

#ifdef MDNS_STATIC_CONSTRUCTOR

namespace lwip { namespace mdns {

Responder::static_constructor Responder::_sc;
    
}}

#endif
#endif