static void conn_to_str(const conn *c, char *buf) {
    char addr_text[MAXPATHLEN];

    if (!c) {
        strcpy(buf, "<null>");
    } else if (c->state == conn_closed) {
        strcpy(buf, "<closed>");
    } else {
        const char *protoname = "?";
        struct sockaddr_in6 local_addr;
        struct sockaddr *addr = (void *)&c->request_addr;
        int af;
        unsigned short port = 0;

        /* For listen ports and idle UDP ports, show listen address */
        if (c->state == conn_listening ||
                (IS_UDP(c->transport) &&
                 c->state == conn_read)) {
            socklen_t local_addr_len = sizeof(local_addr);

            if (getsockname(c->sfd,
                        (struct sockaddr *)&local_addr,
                        &local_addr_len) == 0) {
                addr = (struct sockaddr *)&local_addr;
            }
        }

        af = addr->sa_family;
        addr_text[0] = '\0';

        switch (af) {
            case AF_INET:
                (void) inet_ntop(af,
                        &((struct sockaddr_in *)addr)->sin_addr,
                        addr_text,
                        sizeof(addr_text) - 1);
                port = ntohs(((struct sockaddr_in *)addr)->sin_port);
                protoname = IS_UDP(c->transport) ? "udp" : "tcp";
                break;

            case AF_INET6:
                addr_text[0] = '[';
                addr_text[1] = '\0';
                if (inet_ntop(af,
                        &((struct sockaddr_in6 *)addr)->sin6_addr,
                        addr_text + 1,
                        sizeof(addr_text) - 2)) {
                    strcat(addr_text, "]");
                }
                port = ntohs(((struct sockaddr_in6 *)addr)->sin6_port);
                protoname = IS_UDP(c->transport) ? "udp6" : "tcp6";
                break;

            case AF_UNIX:
                strncpy(addr_text,
                        ((struct sockaddr_un *)addr)->sun_path,
                        sizeof(addr_text) - 1);
                addr_text[sizeof(addr_text)-1] = '\0';
                protoname = "unix";
                break;
        }

        if (strlen(addr_text) < 2) {
            /* Most likely this is a connected UNIX-domain client which
             * has no peer socket address, but there's no portable way
             * to tell for sure.
             */
            sprintf(addr_text, "<AF %d>", af);
        }

        if (port) {
            sprintf(buf, "%s:%s:%u", protoname, addr_text, port);
        } else {
            sprintf(buf, "%s:%s", protoname, addr_text);
        }
    }
}
