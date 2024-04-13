format_timestamp(const uint32_t i)
{
    static char buf[sizeof("0000.000000s")];
    snprintf(buf, sizeof(buf), "%u.%06us", i / 1000000, i % 1000000);
    return buf;
}
