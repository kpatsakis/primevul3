static inline void lockSearcher()
{
#ifndef NDEBUG
    ASSERT(!searcherInUse);
    searcherInUse = true;
#endif
}
