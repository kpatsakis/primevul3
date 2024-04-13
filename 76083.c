void JSArray::sortNumeric(ExecState* exec, JSValue compareFunction, CallType callType, const CallData& callData)
{
    ArrayStorage* storage = m_storage;

    unsigned lengthNotIncludingUndefined = compactForSorting();
    if (storage->m_sparseValueMap) {
        throwOutOfMemoryError(exec);
        return;
    }

    if (!lengthNotIncludingUndefined)
        return;
        
    bool allValuesAreNumbers = true;
    size_t size = storage->m_numValuesInVector;
    for (size_t i = 0; i < size; ++i) {
        if (!storage->m_vector[i].isNumber()) {
            allValuesAreNumbers = false;
            break;
        }
    }

    if (!allValuesAreNumbers)
        return sort(exec, compareFunction, callType, callData);

    qsort(storage->m_vector, size, sizeof(JSValue), compareNumbersForQSort);

    checkConsistency(SortConsistencyCheck);
}
