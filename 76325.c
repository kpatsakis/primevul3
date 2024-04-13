bool InlineTextBox::containsCaretOffset(int offset) const
{
    if (offset < m_start)
        return false;

    int pastEnd = m_start + m_len;

    if (offset < pastEnd)
        return true;

    if (offset > pastEnd)
        return false;

    if (isLineBreak())
        return false;

    return true;
}
