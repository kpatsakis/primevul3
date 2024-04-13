static void remove_stream(FFServerStream *stream)
{
    FFServerStream **ps;
    ps = &config.first_stream;
    while (*ps) {
        if (*ps == stream)
            *ps = (*ps)->next;
        else
            ps = &(*ps)->next;
    }
}
