JSArray::JSArray(JSGlobalData& globalData, Structure* structure, unsigned initialLength, ArrayCreationMode creationMode)
    : JSNonFinalObject(globalData, structure)
{
    ASSERT(inherits(&s_info));

    unsigned initialCapacity;
    if (creationMode == CreateCompact)
        initialCapacity = initialLength;
    else
        initialCapacity = min(BASE_VECTOR_LEN, MIN_SPARSE_ARRAY_INDEX);
    
    m_storage = static_cast<ArrayStorage*>(fastMalloc(storageSize(initialCapacity)));
    m_storage->m_allocBase = m_storage;
    m_storage->m_length = initialLength;
    m_indexBias = 0;
    m_vectorLength = initialCapacity;
    m_storage->m_sparseValueMap = 0;
    m_storage->subclassData = 0;
    m_storage->reportedMapCapacity = 0;

    if (creationMode == CreateCompact) {
#if CHECK_ARRAY_CONSISTENCY
        m_storage->m_inCompactInitialization = !!initialCapacity;
#endif
        m_storage->m_length = 0;
        m_storage->m_numValuesInVector = initialCapacity;
    } else {
#if CHECK_ARRAY_CONSISTENCY
        storage->m_inCompactInitialization = false;
#endif
        m_storage->m_length = initialLength;
        m_storage->m_numValuesInVector = 0;
        WriteBarrier<Unknown>* vector = m_storage->m_vector;
        for (size_t i = 0; i < initialCapacity; ++i)
            vector[i].clear();
    }

    checkConsistency();
    
    Heap::heap(this)->reportExtraMemoryCost(storageSize(initialCapacity));
}
