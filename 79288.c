UChar TextIterator::characterAt(unsigned index) const
{
    ASSERT_WITH_SECURITY_IMPLICATION(index < static_cast<unsigned>(length()));
    if (!(index < static_cast<unsigned>(length())))
        return 0;

    if (!m_textCharacters)
        return string()[startOffset() + index];

    return m_textCharacters[index];
}
