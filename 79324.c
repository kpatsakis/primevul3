inline bool SearchBuffer::isWordStartMatch(size_t start, size_t length) const
{
    ASSERT(m_options & AtWordStarts);

    if (!start)
        return true;

    int size = m_buffer.size();
    int offset = start;
    UChar32 firstCharacter;
    U16_GET(m_buffer.data(), 0, offset, size, firstCharacter);

    if (m_options & TreatMedialCapitalAsWordStart) {
        UChar32 previousCharacter;
        U16_PREV(m_buffer.data(), 0, offset, previousCharacter);

        if (isSeparator(firstCharacter)) {
            if (!isSeparator(previousCharacter))
                return true;
        } else if (isASCIIUpper(firstCharacter)) {
            if (!isASCIIUpper(previousCharacter))
                return true;
            offset = start;
            U16_FWD_1(m_buffer.data(), offset, size);
            UChar32 nextCharacter = 0;
            if (offset < size)
                U16_GET(m_buffer.data(), 0, offset, size, nextCharacter);
            if (!isASCIIUpper(nextCharacter) && !isASCIIDigit(nextCharacter) && !isSeparator(nextCharacter))
                return true;
        } else if (isASCIIDigit(firstCharacter)) {
            if (!isASCIIDigit(previousCharacter))
                return true;
        } else if (isSeparator(previousCharacter) || isASCIIDigit(previousCharacter)) {
            return true;
        }
    }

    if (Font::isCJKIdeographOrSymbol(firstCharacter))
        return true;

    size_t wordBreakSearchStart = start + length;
    while (wordBreakSearchStart > start)
        wordBreakSearchStart = findNextWordFromIndex(m_buffer.data(), m_buffer.size(), wordBreakSearchStart, false /* backwards */);
    return wordBreakSearchStart == start;
}
