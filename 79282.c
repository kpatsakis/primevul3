inline size_t SearchBuffer::append(const UChar* characters, size_t length)
{
    ASSERT(length);

    if (m_atBreak) {
        m_buffer.shrink(0);
        m_prefixLength = 0;
        m_atBreak = false;
    } else if (m_buffer.size() == m_buffer.capacity()) {
        memcpy(m_buffer.data(), m_buffer.data() + m_buffer.size() - m_overlap, m_overlap * sizeof(UChar));
        m_prefixLength -= min(m_prefixLength, m_buffer.size() - m_overlap);
        m_buffer.shrink(m_overlap);
    }

    size_t oldLength = m_buffer.size();
    size_t usableLength = min(m_buffer.capacity() - oldLength, length);
    ASSERT(usableLength);
    m_buffer.append(characters, usableLength);
    foldQuoteMarksAndSoftHyphens(m_buffer.data() + oldLength, usableLength);
    return usableLength;
}
