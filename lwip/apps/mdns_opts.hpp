// To use static constructor means mdns.cpp must compile
// NOT using a static constructor means we
// don't track already-initialized state at all
// so be sure to ONLY CREATE ONE mDNSResponder responder in that case
#define MDNS_INIT_STATIC_CONSTRUCTOR 1
