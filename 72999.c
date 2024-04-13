format_id(const u_char *id)
{
    static char buf[25];
    snprintf(buf, 25, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
             id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]);
    buf[24] = '\0';
    return buf;
}
