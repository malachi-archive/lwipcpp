#include "mdns.hpp"

namespace lwip { namespace mdns {

// Some compilers optimize this out, so possibly we don't even need 
// it
mDNSResponder::static_constructor mDNSResponder::_sc;
    
}}
