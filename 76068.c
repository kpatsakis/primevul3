bool JSArray::increaseVectorLength(unsigned newLength)
{

    ArrayStorage* storage = m_storage;

    unsigned vectorLength = m_vectorLength;
    ASSERT(newLength > vectorLength);
    ASSERT(newLength <= MAX_STORAGE_VECTOR_INDEX);
    unsigned newVectorLength = getNewVectorLength(newLength);
    void* baseStorage = storage->m_allocBase;

    if (!tryFastRealloc(baseStorage, storageSize(newVectorLength + m_indexBias)).getValue(baseStorage))
        return false;

    storage = m_storage = reinterpret_cast_ptr<ArrayStorage*>(static_cast<char*>(baseStorage) + m_indexBias * sizeof(JSValue));
    m_storage->m_allocBase = baseStorage;

    WriteBarrier<Unknown>* vector = storage->m_vector;
    for (unsigned i = vectorLength; i < newVectorLength; ++i)
        vector[i].clear();

    m_vectorLength = newVectorLength;
    
    Heap::heap(this)->reportExtraMemoryCost(storageSize(newVectorLength) - storageSize(vectorLength));

    return true;
}
