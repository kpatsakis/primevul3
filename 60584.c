static void log_connection(HTTPContext *c)
{
    if (c->suppress_log)
        return;

    http_log("%s - - [%s] \"%s %s\" %d %"PRId64"\n",
             inet_ntoa(c->from_addr.sin_addr), c->method, c->url,
             c->protocol, (c->http_error ? c->http_error : 200), c->data_count);
}
