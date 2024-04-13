bool JSArray::deleteProperty(ExecState* exec, unsigned i)
{
    checkConsistency();

    ArrayStorage* storage = m_storage;
    
    if (i < m_vectorLength) {
        WriteBarrier<Unknown>& valueSlot = storage->m_vector[i];
        if (!valueSlot) {
            checkConsistency();
            return false;
        }
        valueSlot.clear();
        --storage->m_numValuesInVector;
        checkConsistency();
        return true;
    }

    if (SparseArrayValueMap* map = storage->m_sparseValueMap) {
        if (i >= MIN_SPARSE_ARRAY_INDEX) {
            SparseArrayValueMap::iterator it = map->find(i);
            if (it != map->end()) {
                map->remove(it);
                checkConsistency();
                return true;
            }
        }
    }

    checkConsistency();

    if (i > MAX_ARRAY_INDEX)
        return deleteProperty(exec, Identifier::from(exec, i));

    return false;
}
