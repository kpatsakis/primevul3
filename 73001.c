format_interval_update(const uint16_t i)
{
    return i == 0xFFFF ? "infinity" : format_interval(i);
}
