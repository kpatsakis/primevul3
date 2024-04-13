format_interval(const uint16_t i)
{
    static char buf[sizeof("000.00s")];

    if (i == 0)
        return "0.0s (bogus)";
    snprintf(buf, sizeof(buf), "%u.%02us", i / 100, i % 100);
    return buf;
}
