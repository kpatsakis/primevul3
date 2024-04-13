static int server_sockets(int port, enum network_transport transport,
                          FILE *portnumber_file) {
    if (settings.inter == NULL) {
        return server_socket(settings.inter, port, transport, portnumber_file);
    } else {
        char *b;
        int ret = 0;
        char *list = strdup(settings.inter);

        if (list == NULL) {
            fprintf(stderr, "Failed to allocate memory for parsing server interface string\n");
            return 1;
        }
        for (char *p = strtok_r(list, ";,", &b);
             p != NULL;
             p = strtok_r(NULL, ";,", &b)) {
            int the_port = port;

            char *h = NULL;
            if (*p == '[') {
                char *e = strchr(p, ']');
                if (e == NULL) {
                    fprintf(stderr, "Invalid IPV6 address: \"%s\"", p);
                    return 1;
                }
                h = ++p; // skip the opening '['
                *e = '\0';
                p = ++e; // skip the closing ']'
            }

            char *s = strchr(p, ':');
            if (s != NULL) {
                if (strchr(s + 1, ':') == NULL || h != NULL) {
                    *s = '\0';
                    ++s;
                    if (!safe_strtol(s, &the_port)) {
                        fprintf(stderr, "Invalid port number: \"%s\"", s);
                        return 1;
                    }
                }
            }

            if (h != NULL)
                p = h;

            if (strcmp(p, "*") == 0) {
                p = NULL;
            }
            ret |= server_socket(p, the_port, transport, portnumber_file);
        }
        free(list);
        return ret;
    }
}
