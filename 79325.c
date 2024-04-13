int WordAwareIterator::length() const
{
    if (!m_buffer.isEmpty())
        return m_buffer.size();
    if (m_previousText)
        return m_previousLength;
    return m_textIterator.length();
}
