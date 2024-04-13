static inline void ensureCharacterGetsLineBox(LineMidpointState& lineMidpointState, InlineIterator& textParagraphSeparator)
{
    InlineIterator midpoint(0, textParagraphSeparator.m_obj, textParagraphSeparator.m_pos);
    startIgnoringSpaces(lineMidpointState, InlineIterator(0, textParagraphSeparator.m_obj, textParagraphSeparator.m_pos - 1));
    stopIgnoringSpaces(lineMidpointState, InlineIterator(0, textParagraphSeparator.m_obj, textParagraphSeparator.m_pos));
}
