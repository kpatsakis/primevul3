bool JSArray::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    if (propertyName == exec->propertyNames().length) {
        descriptor.setDescriptor(jsNumber(length()), DontDelete | DontEnum);
        return true;
    }

    ArrayStorage* storage = m_storage;
    
    bool isArrayIndex;
    unsigned i = propertyName.toArrayIndex(isArrayIndex);
    if (isArrayIndex) {
        if (i >= storage->m_length)
            return false;
        if (i < m_vectorLength) {
            WriteBarrier<Unknown>& value = storage->m_vector[i];
            if (value) {
                descriptor.setDescriptor(value.get(), 0);
                return true;
            }
        } else if (SparseArrayValueMap* map = storage->m_sparseValueMap) {
            if (i >= MIN_SPARSE_ARRAY_INDEX) {
                SparseArrayValueMap::iterator it = map->find(i);
                if (it != map->end()) {
                    descriptor.setDescriptor(it->second.get(), 0);
                    return true;
                }
            }
        }
    }
    return JSObject::getOwnPropertyDescriptor(exec, propertyName, descriptor);
}
