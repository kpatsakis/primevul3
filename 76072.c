void JSArray::push(ExecState* exec, JSValue value)
{
    checkConsistency();
    
    ArrayStorage* storage = m_storage;

    if (UNLIKELY(storage->m_length == 0xFFFFFFFFu)) {
        put(exec, storage->m_length, value);
        throwError(exec, createRangeError(exec, "Invalid array length"));
        return;
    }

    if (storage->m_length < m_vectorLength) {
        storage->m_vector[storage->m_length].set(exec->globalData(), this, value);
        ++storage->m_numValuesInVector;
        ++storage->m_length;
        checkConsistency();
        return;
    }

    if (storage->m_length < MIN_SPARSE_ARRAY_INDEX) {
        SparseArrayValueMap* map = storage->m_sparseValueMap;
        if (!map || map->isEmpty()) {
            if (increaseVectorLength(storage->m_length + 1)) {
                storage = m_storage;
                storage->m_vector[storage->m_length].set(exec->globalData(), this, value);
                ++storage->m_numValuesInVector;
                ++storage->m_length;
                checkConsistency();
                return;
            }
            checkConsistency();
            throwOutOfMemoryError(exec);
            return;
        }
    }

    putSlowCase(exec, storage->m_length++, value);
}
