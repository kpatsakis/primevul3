inline void SearchBuffer::append(UChar c, bool isStart)
{
    m_buffer[m_cursor] = c == noBreakSpace ? ' ' : foldQuoteMarkOrSoftHyphen(c);
    m_isCharacterStartBuffer[m_cursor] = isStart;
    if (++m_cursor == m_target.length()) {
        m_cursor = 0;
        m_isBufferFull = true;
    }
}
