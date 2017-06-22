#include "mdns.hpp"

#ifdef MDNS_STATIC_CONSTRUCTOR

namespace lwip { namespace mdns {

Responder::static_constructor Responder::_sc;
    
}}

#endif
