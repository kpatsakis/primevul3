void WordAwareIterator::advance()
{
    m_previousText = 0;
    m_buffer.clear();      // toss any old buffer we built up

    if (!m_didLookAhead) {
        ASSERT(!m_textIterator.atEnd());
        m_textIterator.advance();
    }
    m_didLookAhead = false;

    while (!m_textIterator.atEnd() && m_textIterator.length() == 0)
        m_textIterator.advance();
    m_range = m_textIterator.range();

    if (m_textIterator.atEnd())
        return;
    
    while (1) {
        if (isSpaceOrNewline(m_textIterator.characters()[m_textIterator.length() - 1]))
            return;

        if (m_buffer.isEmpty()) {
            m_previousText = m_textIterator.characters();
            m_previousLength = m_textIterator.length();
        }

        m_textIterator.advance();
        if (m_textIterator.atEnd() || m_textIterator.length() == 0 || isSpaceOrNewline(m_textIterator.characters()[0])) {
            m_didLookAhead = true;
            return;
        }

        if (m_buffer.isEmpty()) {
            m_buffer.append(m_previousText, m_previousLength);
            m_previousText = 0;
        }
        m_buffer.append(m_textIterator.characters(), m_textIterator.length());
        int exception = 0;
        m_range->setEnd(m_textIterator.range()->endContainer(), m_textIterator.range()->endOffset(), exception);
    }
}
