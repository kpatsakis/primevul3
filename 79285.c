void TextIterator::appendTextToStringBuilder(StringBuilder& builder) const
{
    if (!m_textCharacters)
        builder.append(string(), startOffset(), length());
    else
        builder.append(characters(), length());
}
