void JSArray::sort(ExecState* exec)
{
    ArrayStorage* storage = m_storage;

    unsigned lengthNotIncludingUndefined = compactForSorting();
    if (storage->m_sparseValueMap) {
        throwOutOfMemoryError(exec);
        return;
    }

    if (!lengthNotIncludingUndefined)
        return;


    Vector<ValueStringPair> values(lengthNotIncludingUndefined);
    if (!values.begin()) {
        throwOutOfMemoryError(exec);
        return;
    }
    
    Heap::heap(this)->pushTempSortVector(&values);

    for (size_t i = 0; i < lengthNotIncludingUndefined; i++) {
        JSValue value = storage->m_vector[i].get();
        ASSERT(!value.isUndefined());
        values[i].first = value;
    }


    for (size_t i = 0; i < lengthNotIncludingUndefined; i++)
        values[i].second = values[i].first.toString(exec);

    if (exec->hadException()) {
        Heap::heap(this)->popTempSortVector(&values);
        return;
    }


#if HAVE(MERGESORT)
    mergesort(values.begin(), values.size(), sizeof(ValueStringPair), compareByStringPairForQSort);
#else
    qsort(values.begin(), values.size(), sizeof(ValueStringPair), compareByStringPairForQSort);
#endif

    if (m_vectorLength < lengthNotIncludingUndefined)
        increaseVectorLength(lengthNotIncludingUndefined);
    if (storage->m_length < lengthNotIncludingUndefined)
        storage->m_length = lengthNotIncludingUndefined;

    JSGlobalData& globalData = exec->globalData();
    for (size_t i = 0; i < lengthNotIncludingUndefined; i++)
        storage->m_vector[i].set(globalData, this, values[i].first);

    Heap::heap(this)->popTempSortVector(&values);
    
    checkConsistency(SortConsistencyCheck);
}
