unsigned JSArray::compactForSorting()
{
    checkConsistency();

    ArrayStorage* storage = m_storage;

    unsigned usedVectorLength = min(storage->m_length, m_vectorLength);

    unsigned numDefined = 0;
    unsigned numUndefined = 0;

    for (; numDefined < usedVectorLength; ++numDefined) {
        JSValue v = storage->m_vector[numDefined].get();
        if (!v || v.isUndefined())
            break;
    }

    for (unsigned i = numDefined; i < usedVectorLength; ++i) {
        JSValue v = storage->m_vector[i].get();
        if (v) {
            if (v.isUndefined())
                ++numUndefined;
            else
                storage->m_vector[numDefined++].setWithoutWriteBarrier(v);
        }
    }

    unsigned newUsedVectorLength = numDefined + numUndefined;

    if (SparseArrayValueMap* map = storage->m_sparseValueMap) {
        newUsedVectorLength += map->size();
        if (newUsedVectorLength > m_vectorLength) {
            if ((newUsedVectorLength > MAX_STORAGE_VECTOR_LENGTH) || !increaseVectorLength(newUsedVectorLength))
                return 0;

            storage = m_storage;
        }

        SparseArrayValueMap::iterator end = map->end();
        for (SparseArrayValueMap::iterator it = map->begin(); it != end; ++it)
            storage->m_vector[numDefined++].setWithoutWriteBarrier(it->second.get());

        delete map;
        storage->m_sparseValueMap = 0;
    }

    for (unsigned i = numDefined; i < newUsedVectorLength; ++i)
        storage->m_vector[i].setUndefined();
    for (unsigned i = newUsedVectorLength; i < usedVectorLength; ++i)
        storage->m_vector[i].clear();

    storage->m_numValuesInVector = newUsedVectorLength;

    checkConsistency(SortConsistencyCheck);

    return numDefined;
}
