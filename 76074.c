NEVER_INLINE void JSArray::putSlowCase(ExecState* exec, unsigned i, JSValue value)
{
    ArrayStorage* storage = m_storage;
    
    SparseArrayValueMap* map = storage->m_sparseValueMap;

    if (i >= MIN_SPARSE_ARRAY_INDEX) {
        if (i > MAX_ARRAY_INDEX) {
            PutPropertySlot slot;
            put(exec, Identifier::from(exec, i), value, slot);
            return;
        }

        if ((i > MAX_STORAGE_VECTOR_INDEX) || !isDenseEnoughForVector(i + 1, storage->m_numValuesInVector + 1)) {
            if (!map) {
                map = new SparseArrayValueMap;
                storage->m_sparseValueMap = map;
            }

            WriteBarrier<Unknown> temp;
            pair<SparseArrayValueMap::iterator, bool> result = map->add(i, temp);
            result.first->second.set(exec->globalData(), this, value);
            if (!result.second) // pre-existing entry
                return;

            size_t capacity = map->capacity();
            if (capacity != storage->reportedMapCapacity) {
                Heap::heap(this)->reportExtraMemoryCost((capacity - storage->reportedMapCapacity) * (sizeof(unsigned) + sizeof(JSValue)));
                storage->reportedMapCapacity = capacity;
            }
            return;
        }
    }

    if (!map || map->isEmpty()) {
        if (increaseVectorLength(i + 1)) {
            storage = m_storage;
            storage->m_vector[i].set(exec->globalData(), this, value);
            ++storage->m_numValuesInVector;
            checkConsistency();
        } else
            throwOutOfMemoryError(exec);
        return;
    }

    unsigned newNumValuesInVector = storage->m_numValuesInVector + 1;
    unsigned newVectorLength = getNewVectorLength(i + 1);
    for (unsigned j = max(m_vectorLength, MIN_SPARSE_ARRAY_INDEX); j < newVectorLength; ++j)
        newNumValuesInVector += map->contains(j);
    if (i >= MIN_SPARSE_ARRAY_INDEX)
        newNumValuesInVector -= map->contains(i);
    if (isDenseEnoughForVector(newVectorLength, newNumValuesInVector)) {
        unsigned needLength = max(i + 1, storage->m_length);
        unsigned proposedNewNumValuesInVector = newNumValuesInVector;
        while ((newVectorLength < needLength) && (newVectorLength < MAX_STORAGE_VECTOR_LENGTH)) {
            unsigned proposedNewVectorLength = getNewVectorLength(newVectorLength + 1);
            for (unsigned j = max(newVectorLength, MIN_SPARSE_ARRAY_INDEX); j < proposedNewVectorLength; ++j)
                proposedNewNumValuesInVector += map->contains(j);
            if (!isDenseEnoughForVector(proposedNewVectorLength, proposedNewNumValuesInVector))
                break;
            newVectorLength = proposedNewVectorLength;
            newNumValuesInVector = proposedNewNumValuesInVector;
        }
    }

    void* baseStorage = storage->m_allocBase;
    
    if (!tryFastRealloc(baseStorage, storageSize(newVectorLength + m_indexBias)).getValue(baseStorage)) {
        throwOutOfMemoryError(exec);
        return;
    }

    m_storage = reinterpret_cast_ptr<ArrayStorage*>(static_cast<char*>(baseStorage) + m_indexBias * sizeof(JSValue));
    m_storage->m_allocBase = baseStorage;
    storage = m_storage;
    
    unsigned vectorLength = m_vectorLength;
    WriteBarrier<Unknown>* vector = storage->m_vector;

    if (newNumValuesInVector == storage->m_numValuesInVector + 1) {
        for (unsigned j = vectorLength; j < newVectorLength; ++j)
            vector[j].clear();
        if (i > MIN_SPARSE_ARRAY_INDEX)
            map->remove(i);
    } else {
        for (unsigned j = vectorLength; j < max(vectorLength, MIN_SPARSE_ARRAY_INDEX); ++j)
            vector[j].clear();
        JSGlobalData& globalData = exec->globalData();
        for (unsigned j = max(vectorLength, MIN_SPARSE_ARRAY_INDEX); j < newVectorLength; ++j)
            vector[j].set(globalData, this, map->take(j).get());
    }

    ASSERT(i < newVectorLength);

    m_vectorLength = newVectorLength;
    storage->m_numValuesInVector = newNumValuesInVector;

    storage->m_vector[i].set(exec->globalData(), this, value);

    checkConsistency();

    Heap::heap(this)->reportExtraMemoryCost(storageSize(newVectorLength) - storageSize(vectorLength));
}
