void JSArray::shiftCount(ExecState* exec, int count)
{
    ASSERT(count > 0);
    
    ArrayStorage* storage = m_storage;
    
    unsigned oldLength = storage->m_length;
    
    if (!oldLength)
        return;
    
    if (oldLength != storage->m_numValuesInVector) {
        for (unsigned i = count; i < oldLength; ++i) {
            if ((i >= m_vectorLength) || (!m_storage->m_vector[i])) {
                PropertySlot slot(this);
                JSValue p = prototype();
                if ((!p.isNull()) && (asObject(p)->getPropertySlot(exec, i, slot)))
                    put(exec, i, slot.getValue(exec, i));
            }
        }

        storage = m_storage; // The put() above could have grown the vector and realloc'ed storage.

        for (unsigned i = 0; i < (unsigned)count; ++i)
            if ((i < m_vectorLength) && (storage->m_vector[i]))
                --storage->m_numValuesInVector;
    } else
        storage->m_numValuesInVector -= count;
    
    storage->m_length -= count;
    
    if (m_vectorLength) {
        count = min(m_vectorLength, (unsigned)count);
        
        m_vectorLength -= count;
        
        if (m_vectorLength) {
            char* newBaseStorage = reinterpret_cast<char*>(storage) + count * sizeof(JSValue);
            memmove(newBaseStorage, storage, storageSize(0));
            m_storage = reinterpret_cast_ptr<ArrayStorage*>(newBaseStorage);

            m_indexBias += count;
        }
    }
}
