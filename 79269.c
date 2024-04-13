BackwardsCharacterIterator::BackwardsCharacterIterator(const Range* range, TextIteratorBehavior behavior)
    : m_offset(0)
    , m_runOffset(0)
    , m_atBreak(true)
    , m_textIterator(range, behavior)
{
    while (!atEnd() && !m_textIterator.length())
        m_textIterator.advance();
}
