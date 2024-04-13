void JSArray::setLength(unsigned newLength)
{
    ArrayStorage* storage = m_storage;
    
#if CHECK_ARRAY_CONSISTENCY
    if (!storage->m_inCompactInitialization)
        checkConsistency();
    else
        storage->m_inCompactInitialization = false;
#endif

    unsigned length = storage->m_length;

    if (newLength < length) {
        unsigned usedVectorLength = min(length, m_vectorLength);
        for (unsigned i = newLength; i < usedVectorLength; ++i) {
            WriteBarrier<Unknown>& valueSlot = storage->m_vector[i];
            bool hadValue = valueSlot;
            valueSlot.clear();
            storage->m_numValuesInVector -= hadValue;
        }

        if (SparseArrayValueMap* map = storage->m_sparseValueMap) {
            SparseArrayValueMap copy = *map;
            SparseArrayValueMap::iterator end = copy.end();
            for (SparseArrayValueMap::iterator it = copy.begin(); it != end; ++it) {
                if (it->first >= newLength)
                    map->remove(it->first);
            }
            if (map->isEmpty()) {
                delete map;
                storage->m_sparseValueMap = 0;
            }
        }
    }

    storage->m_length = newLength;

    checkConsistency();
}
