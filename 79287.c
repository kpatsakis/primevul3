inline bool SearchBuffer::atBreak() const
{
    return !m_cursor && !m_isBufferFull;
}
