reduce_timeout(uint16_t max, uint16_t *timeout)
{
    if (max && (!*timeout || *timeout > max)) {
        *timeout = max;
    }
}
