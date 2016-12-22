#pragma once

extern "C"
{
    #include "lwip/api.h"
}

namespace lwip
{
    class Netconn
    {
        struct netconn *conn;

    public:
        Netconn(struct netconn* conn) : conn(conn) {}

        Netconn(enum netconn_type t, uint8_t proto = 0, netconn_callback callback = NULL)
        {
            conn = netconn_new_with_proto_and_callback(t, proto, callback);
        }

        err_t bind(ip_addr_t* addr, u16_t port)
        {
            return netconn_bind(conn, addr, port);
        }

        err_t listen(u8_t backlog = TCP_DEFAULT_LISTEN_BACKLOG)
        {
            return netconn_listen_with_backlog(conn, backlog);
        }

        err_t accept(struct netconn** new_conn)
        {
            return netconn_accept(conn, new_conn);
        }

        /*
        err_t delete()
        {
            netconn_delete(conn);
        }*/

        err_t peer(ip_addr_t* addr, u16_t* port)
        {
            return netconn_peer(conn, addr, port);
        }

        err_t write(const void *dataptr, size_t size, u8_t apiflags, size_t *bytes_written = NULL)
        {
            return netconn_write_partly(conn, dataptr, size, apiflags, bytes_written);
        }
    };
}
