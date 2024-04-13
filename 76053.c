JSArray::JSArray(JSGlobalData& globalData, Structure* structure)
    : JSNonFinalObject(globalData, structure)
{
    ASSERT(inherits(&s_info));

    unsigned initialCapacity = 0;

    m_storage = static_cast<ArrayStorage*>(fastZeroedMalloc(storageSize(initialCapacity)));
    m_storage->m_allocBase = m_storage;
    m_indexBias = 0;
    m_vectorLength = initialCapacity;

    checkConsistency();

    Heap::heap(this)->reportExtraMemoryCost(storageSize(0));
}
