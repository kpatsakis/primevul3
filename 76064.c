bool JSArray::getOwnPropertySlot(ExecState* exec, unsigned i, PropertySlot& slot)
{
    ArrayStorage* storage = m_storage;
    
    if (i >= storage->m_length) {
        if (i > MAX_ARRAY_INDEX)
            return getOwnPropertySlot(exec, Identifier::from(exec, i), slot);
        return false;
    }

    if (i < m_vectorLength) {
        JSValue value = storage->m_vector[i].get();
        if (value) {
            slot.setValue(value);
            return true;
        }
    } else if (SparseArrayValueMap* map = storage->m_sparseValueMap) {
        if (i >= MIN_SPARSE_ARRAY_INDEX) {
            SparseArrayValueMap::iterator it = map->find(i);
            if (it != map->end()) {
                slot.setValue(it->second.get());
                return true;
            }
        }
    }

    return JSObject::getOwnPropertySlot(exec, Identifier::from(exec, i), slot);
}
