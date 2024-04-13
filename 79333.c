static void normalizeCharacters(const UChar* characters, unsigned length, Vector<UChar>& buffer)
{
    ASSERT(length);

    buffer.resize(length);

    UErrorCode status = U_ZERO_ERROR;
    size_t bufferSize = unorm_normalize(characters, length, UNORM_NFC, 0, buffer.data(), length, &status);
    ASSERT(status == U_ZERO_ERROR || status == U_STRING_NOT_TERMINATED_WARNING || status == U_BUFFER_OVERFLOW_ERROR);
    ASSERT(bufferSize);

    buffer.resize(bufferSize);

    if (status == U_ZERO_ERROR || status == U_STRING_NOT_TERMINATED_WARNING)
        return;

    status = U_ZERO_ERROR;
    unorm_normalize(characters, length, UNORM_NFC, 0, buffer.data(), bufferSize, &status);
    ASSERT(status == U_STRING_NOT_TERMINATED_WARNING);
}
