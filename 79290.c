const UChar* WordAwareIterator::characters() const
{
    if (!m_buffer.isEmpty())
        return m_buffer.data();
    if (m_previousText)
        return m_previousText;
    return m_textIterator.characters();
}
