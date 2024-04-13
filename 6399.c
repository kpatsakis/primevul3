establish_http_connection (struct conn_s *connptr, struct request_s *request)
{
        char portbuff[7];
        char dst[sizeof(struct in6_addr)];

        /* Build a port string if it's not a standard port */
        if (request->port != HTTP_PORT && request->port != HTTP_PORT_SSL)
                snprintf (portbuff, 7, ":%u", request->port);
        else
                portbuff[0] = '\0';

        if (inet_pton(AF_INET6, request->host, dst) > 0) {
                /* host is an IPv6 address literal, so surround it with
                 * [] */
                return write_message (connptr->server_fd,
                                      "%s %s HTTP/1.%u\r\n"
                                      "Host: [%s]%s\r\n"
                                      "Connection: close\r\n",
                                      request->method, request->path,
                                      connptr->protocol.major != 1 ? 0 :
                                               connptr->protocol.minor,
                                      request->host, portbuff);
        } else if (connptr->upstream_proxy &&
                   connptr->upstream_proxy->type == PT_HTTP &&
                   connptr->upstream_proxy->ua.authstr) {
                return write_message (connptr->server_fd,
                                      "%s %s HTTP/1.%u\r\n"
                                      "Host: %s%s\r\n"
                                      "Connection: close\r\n"
                                      "Proxy-Authorization: Basic %s\r\n",
                                      request->method, request->path,
                                      connptr->protocol.major != 1 ? 0 :
                                               connptr->protocol.minor,
                                      request->host, portbuff,
                                      connptr->upstream_proxy->ua.authstr);
        } else {
                return write_message (connptr->server_fd,
                                      "%s %s HTTP/1.%u\r\n"
                                      "Host: %s%s\r\n"
                                      "Connection: close\r\n",
                                      request->method, request->path,
                                      connptr->protocol.major != 1 ? 0 :
                                               connptr->protocol.minor,
                                      request->host, portbuff);
        }
}