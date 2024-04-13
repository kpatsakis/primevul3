inline size_t SearchBuffer::search(size_t& start)
{
    if (!m_isBufferFull)
        return 0;
    if (!m_isCharacterStartBuffer[m_cursor])
        return 0;

    size_t tailSpace = m_target.length() - m_cursor;
    if (memcmp(&m_buffer[m_cursor], m_target.characters(), tailSpace * sizeof(UChar)) != 0)
        return 0;
    if (memcmp(&m_buffer[0], m_target.characters() + tailSpace, m_cursor * sizeof(UChar)) != 0)
        return 0;

    start = length();

    m_isCharacterStartBuffer[m_cursor] = false;

    return start;
}
