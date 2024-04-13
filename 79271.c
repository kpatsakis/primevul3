CharacterIterator::CharacterIterator(const Range* r, TextIteratorBehavior behavior)
    : m_offset(0)
    , m_runOffset(0)
    , m_atBreak(true)
    , m_textIterator(r, behavior)
{
    while (!atEnd() && m_textIterator.length() == 0)
        m_textIterator.advance();
}
