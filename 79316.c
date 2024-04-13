void TextIterator::handleTextNodeFirstLetter(RenderTextFragment* renderer)
{
    if (renderer->firstLetter()) {
        RenderObject* r = renderer->firstLetter();
        if (r->style()->visibility() != VISIBLE && !m_ignoresStyleVisibility)
            return;
        if (RenderText* firstLetter = firstRenderTextInFirstLetter(r)) {
            m_handledFirstLetter = true;
            m_remainingTextBox = m_textBox;
            m_textBox = firstLetter->firstTextBox();
            m_sortedTextBoxes.clear();
            m_firstLetterText = firstLetter;
        }
    }
    m_handledFirstLetter = true;
}
