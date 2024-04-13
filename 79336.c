inline void SearchBuffer::prependContext(const UChar* characters, size_t length)
{
    ASSERT(m_needsMoreContext);
    ASSERT(m_prefixLength == m_buffer.size());

    if (!length)
        return;

    m_atBreak = false;

    size_t wordBoundaryContextStart = length;
    if (wordBoundaryContextStart) {
        U16_BACK_1(characters, 0, wordBoundaryContextStart);
        wordBoundaryContextStart = startOfLastWordBoundaryContext(characters, wordBoundaryContextStart);
    }

    size_t usableLength = min(m_buffer.capacity() - m_prefixLength, length - wordBoundaryContextStart);
    m_buffer.prepend(characters + length - usableLength, usableLength);
    m_prefixLength += usableLength;

    if (wordBoundaryContextStart || m_prefixLength == m_buffer.capacity())
        m_needsMoreContext = false;
}
