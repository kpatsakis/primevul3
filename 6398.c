connect_to_upstream (struct conn_s *connptr, struct request_s *request)
{
#ifndef UPSTREAM_SUPPORT
        /*
         * This function does nothing if upstream support was not compiled
         * into tinyproxy.
         */
        return -1;
#else
        char *combined_string;
        int len;

        struct upstream *cur_upstream = connptr->upstream_proxy;

        if (!cur_upstream) {
                log_message (LOG_WARNING,
                             "No upstream proxy defined for %s.",
                             request->host);
                indicate_http_error (connptr, 502,
                                     "Unable to connect to upstream proxy.");
                return -1;
        }

        connptr->server_fd =
            opensock (cur_upstream->host, cur_upstream->port,
                      connptr->server_ip_addr);

        if (connptr->server_fd < 0) {
                log_message (LOG_WARNING,
                             "Could not connect to upstream proxy.");
                indicate_http_error (connptr, 502,
                                     "Unable to connect to upstream proxy",
                                     "detail",
                                     "A network error occurred while trying to "
                                     "connect to the upstream web proxy.",
                                     NULL);
                return -1;
        }

	if (cur_upstream->type != PT_HTTP)
		return connect_to_upstream_proxy(connptr, request);

        log_message (LOG_CONN,
                     "Established connection to upstream proxy \"%s\" "
                     "using file descriptor %d.",
                     cur_upstream->host, connptr->server_fd);

        /*
         * We need to re-write the "path" part of the request so that we
         * can reuse the establish_http_connection() function. It expects a
         * method and path.
         */
        if (connptr->connect_method) {
                len = strlen (request->host) + 7;

                combined_string = (char *) safemalloc (len);
                if (!combined_string) {
                        return -1;
                }

                snprintf (combined_string, len, "%s:%d", request->host,
                          request->port);
        } else {
                len = strlen (request->host) + strlen (request->path) + 14;
                combined_string = (char *) safemalloc (len);
                if (!combined_string) {
                        return -1;
                }

                snprintf (combined_string, len, "http://%s:%d%s", request->host,
                          request->port, request->path);
        }

        if (request->path)
                safefree (request->path);
        request->path = combined_string;

        return establish_http_connection (connptr, request);
#endif
}