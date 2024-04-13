calc_duration(long long int start, long long int now,
              uint32_t *sec, uint32_t *nsec)
{
    long long int msecs = now - start;
    *sec = msecs / 1000;
    *nsec = (msecs % 1000) * (1000 * 1000);
}
