void HarfBuzzShaperBase::setPadding(int padding)
{
    m_padding = padding;
    m_padError = 0;
    if (!m_padding)
        return;

    unsigned numWordEnds = 0;

    for (unsigned i = 0; i < m_normalizedBufferLength; i++) {
        if (isWordEnd(i))
            numWordEnds++;
    }

    if (numWordEnds)
        m_padPerWordBreak = m_padding / numWordEnds;
    else
        m_padPerWordBreak = 0;
}
