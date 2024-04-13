void JSArray::unshiftCount(ExecState* exec, int count)
{
    ArrayStorage* storage = m_storage;

    ASSERT(m_indexBias >= 0);
    ASSERT(count >= 0);
    
    unsigned length = storage->m_length;
    
    if (length != storage->m_numValuesInVector) {
        for (unsigned i = 0; i < length; ++i) {
            if ((i >= m_vectorLength) || (!m_storage->m_vector[i])) {
                PropertySlot slot(this);
                JSValue p = prototype();
                if ((!p.isNull()) && (asObject(p)->getPropertySlot(exec, i, slot)))
                    put(exec, i, slot.getValue(exec, i));
            }
        }
    }
    
    storage = m_storage; // The put() above could have grown the vector and realloc'ed storage.
    
    if (m_indexBias >= count) {
        m_indexBias -= count;
        char* newBaseStorage = reinterpret_cast<char*>(storage) - count * sizeof(JSValue);
        memmove(newBaseStorage, storage, storageSize(0));
        m_storage = reinterpret_cast_ptr<ArrayStorage*>(newBaseStorage);
        m_vectorLength += count;
    } else if (!increaseVectorPrefixLength(m_vectorLength + count)) {
        throwOutOfMemoryError(exec);
        return;
    }

    WriteBarrier<Unknown>* vector = m_storage->m_vector;
    for (int i = 0; i < count; i++)
        vector[i].clear();
}
