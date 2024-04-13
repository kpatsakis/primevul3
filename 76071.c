JSValue JSArray::pop()
{
    checkConsistency();

    ArrayStorage* storage = m_storage;
    
    unsigned length = storage->m_length;
    if (!length)
        return jsUndefined();

    --length;

    JSValue result;

    if (length < m_vectorLength) {
        WriteBarrier<Unknown>& valueSlot = storage->m_vector[length];
        if (valueSlot) {
            --storage->m_numValuesInVector;
            result = valueSlot.get();
            valueSlot.clear();
        } else
            result = jsUndefined();
    } else {
        result = jsUndefined();
        if (SparseArrayValueMap* map = storage->m_sparseValueMap) {
            SparseArrayValueMap::iterator it = map->find(length);
            if (it != map->end()) {
                result = it->second.get();
                map->remove(it);
                if (map->isEmpty()) {
                    delete map;
                    storage->m_sparseValueMap = 0;
                }
            }
        }
    }

    storage->m_length = length;

    checkConsistency();

    return result;
}
