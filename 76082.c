void JSArray::sort(ExecState* exec, JSValue compareFunction, CallType callType, const CallData& callData)
{
    checkConsistency();

    ArrayStorage* storage = m_storage;


    ASSERT(storage->m_length <= static_cast<unsigned>(std::numeric_limits<int>::max()));
    if (storage->m_length > static_cast<unsigned>(std::numeric_limits<int>::max()))
        return;

    unsigned usedVectorLength = min(storage->m_length, m_vectorLength);
    unsigned nodeCount = usedVectorLength + (storage->m_sparseValueMap ? storage->m_sparseValueMap->size() : 0);

    if (!nodeCount)
        return;

    AVLTree<AVLTreeAbstractorForArrayCompare, 44> tree; // Depth 44 is enough for 2^31 items
    tree.abstractor().m_exec = exec;
    tree.abstractor().m_compareFunction = compareFunction;
    tree.abstractor().m_compareCallType = callType;
    tree.abstractor().m_compareCallData = &callData;
    tree.abstractor().m_globalThisValue = exec->globalThisValue();
    tree.abstractor().m_nodes.grow(nodeCount);

    if (callType == CallTypeJS)
        tree.abstractor().m_cachedCall = adoptPtr(new CachedCall(exec, asFunction(compareFunction), 2));

    if (!tree.abstractor().m_nodes.begin()) {
        throwOutOfMemoryError(exec);
        return;
    }


    unsigned numDefined = 0;
    unsigned numUndefined = 0;

    for (; numDefined < usedVectorLength; ++numDefined) {
        JSValue v = storage->m_vector[numDefined].get();
        if (!v || v.isUndefined())
            break;
        tree.abstractor().m_nodes[numDefined].value = v;
        tree.insert(numDefined);
    }
    for (unsigned i = numDefined; i < usedVectorLength; ++i) {
        JSValue v = storage->m_vector[i].get();
        if (v) {
            if (v.isUndefined())
                ++numUndefined;
            else {
                tree.abstractor().m_nodes[numDefined].value = v;
                tree.insert(numDefined);
                ++numDefined;
            }
        }
    }

    unsigned newUsedVectorLength = numDefined + numUndefined;

    if (SparseArrayValueMap* map = storage->m_sparseValueMap) {
        newUsedVectorLength += map->size();
        if (newUsedVectorLength > m_vectorLength) {
            if ((newUsedVectorLength > MAX_STORAGE_VECTOR_LENGTH) || !increaseVectorLength(newUsedVectorLength)) {
                throwOutOfMemoryError(exec);
                return;
            }
        }
        
        storage = m_storage;

        SparseArrayValueMap::iterator end = map->end();
        for (SparseArrayValueMap::iterator it = map->begin(); it != end; ++it) {
            tree.abstractor().m_nodes[numDefined].value = it->second.get();
            tree.insert(numDefined);
            ++numDefined;
        }

        delete map;
        storage->m_sparseValueMap = 0;
    }

    ASSERT(tree.abstractor().m_nodes.size() >= numDefined);


    AVLTree<AVLTreeAbstractorForArrayCompare, 44>::Iterator iter;
    iter.start_iter_least(tree);
    JSGlobalData& globalData = exec->globalData();
    for (unsigned i = 0; i < numDefined; ++i) {
        storage->m_vector[i].set(globalData, this, tree.abstractor().m_nodes[*iter].value);
        ++iter;
    }

    for (unsigned i = numDefined; i < newUsedVectorLength; ++i)
        storage->m_vector[i].setUndefined();

    for (unsigned i = newUsedVectorLength; i < usedVectorLength; ++i)
        storage->m_vector[i].clear();

    storage->m_numValuesInVector = newUsedVectorLength;

    checkConsistency(SortConsistencyCheck);
}
