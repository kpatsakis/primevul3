static int64_t get_server_clock(HTTPContext *c)
{
    /* compute current pts value from system time */
    return (cur_time - c->start_time) * 1000;
}
