int HarfBuzzShaperBase::determineWordBreakSpacing()
{
    int wordBreakSpacing = m_wordSpacingAdjustment;

    if (m_padding > 0) {
        int toPad = roundf(m_padPerWordBreak + m_padError);
        m_padError += m_padPerWordBreak - toPad;

        if (m_padding < toPad)
            toPad = m_padding;
        m_padding -= toPad;
        wordBreakSpacing += toPad;
    }
    return wordBreakSpacing;
}
