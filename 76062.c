ALWAYS_INLINE unsigned JSArray::getNewVectorLength(unsigned desiredLength)
{
    ASSERT(desiredLength <= MAX_STORAGE_VECTOR_LENGTH);

    unsigned increasedLength;
    unsigned maxInitLength = min(m_storage->m_length, 100000U);

    if (desiredLength < maxInitLength)
        increasedLength = maxInitLength;
    else if (!m_vectorLength)
        increasedLength = max(desiredLength, lastArraySize);
    else {
        increasedLength = desiredLength + (desiredLength >> 1) + (desiredLength & 1);
    }

    ASSERT(increasedLength >= desiredLength);

    lastArraySize = min(increasedLength, FIRST_VECTOR_GROW);

    return min(increasedLength, MAX_STORAGE_VECTOR_LENGTH);
}
