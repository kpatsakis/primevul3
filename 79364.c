static inline void unlockSearcher()
{
#ifndef NDEBUG
    ASSERT(searcherInUse);
    searcherInUse = false;
#endif
}
