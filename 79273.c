inline SearchBuffer::SearchBuffer(const String& target, FindOptions options)
    : m_target(options & CaseInsensitive ? target.foldCase() : target)
    , m_options(options)
    , m_buffer(m_target.length())
    , m_isCharacterStartBuffer(m_target.length())
    , m_isBufferFull(false)
    , m_cursor(0)
{
    ASSERT(!m_target.isEmpty());
    m_target.replace(noBreakSpace, ' ');
    foldQuoteMarksAndSoftHyphens(m_target);
}
