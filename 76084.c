static inline size_t storageSize(unsigned vectorLength)
{
    ASSERT(vectorLength <= MAX_STORAGE_VECTOR_LENGTH);

    size_t size = (sizeof(ArrayStorage) - sizeof(JSValue)) + (vectorLength * sizeof(JSValue));
    ASSERT(((size - (sizeof(ArrayStorage) - sizeof(JSValue))) / sizeof(JSValue) == vectorLength) && (size >= (sizeof(ArrayStorage) - sizeof(JSValue))));

    return size;
}
