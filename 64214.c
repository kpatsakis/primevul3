age_secs(long long int age_ms)
{
    return (age_ms < 0 ? 0
            : age_ms >= UINT16_MAX * 1000 ? UINT16_MAX
            : (unsigned int) age_ms / 1000);
}
