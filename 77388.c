static inline int getFocusRingOutset(int offset)
{
#if PLATFORM(CHROMIUM) && OS(DARWIN)
    return offset + 2;
#else
    return 0;
#endif
}
