JSArray::JSArray(JSGlobalData& globalData, Structure* structure, const ArgList& list)
    : JSNonFinalObject(globalData, structure)
{
    ASSERT(inherits(&s_info));

    unsigned initialCapacity = list.size();
    unsigned initialStorage;
    
    if (!initialCapacity)
        initialStorage = 3;
    else
        initialStorage = initialCapacity;
    
    m_storage = static_cast<ArrayStorage*>(fastMalloc(storageSize(initialStorage)));
    m_storage->m_allocBase = m_storage;
    m_indexBias = 0;
    m_storage->m_length = initialCapacity;
    m_vectorLength = initialStorage;
    m_storage->m_numValuesInVector = initialCapacity;
    m_storage->m_sparseValueMap = 0;
    m_storage->subclassData = 0;
    m_storage->reportedMapCapacity = 0;
#if CHECK_ARRAY_CONSISTENCY
    m_storage->m_inCompactInitialization = false;
#endif

    size_t i = 0;
    WriteBarrier<Unknown>* vector = m_storage->m_vector;
    ArgList::const_iterator end = list.end();
    for (ArgList::const_iterator it = list.begin(); it != end; ++it, ++i)
        vector[i].set(globalData, this, *it);
    for (; i < initialStorage; i++)
        vector[i].clear();

    checkConsistency();

    Heap::heap(this)->reportExtraMemoryCost(storageSize(initialStorage));
}
