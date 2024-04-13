bool TextIterator::handleTextNode()
{
    if (m_fullyClippedStack.top() && !m_ignoresStyleVisibility)
        return false;

    RenderText* renderer = toRenderText(m_node->renderer());
        
    m_lastTextNode = m_node;
    String str = renderer->text();

    if (!renderer->style()->collapseWhiteSpace()) {
        int runStart = m_offset;
        if (m_lastTextNodeEndedWithCollapsedSpace && hasVisibleTextNode(renderer)) {
            emitCharacter(' ', m_node, 0, runStart, runStart);
            return false;
        }
        if (!m_handledFirstLetter && renderer->isTextFragment() && !m_offset) {
            handleTextNodeFirstLetter(static_cast<RenderTextFragment*>(renderer));
            if (m_firstLetterText) {
                String firstLetter = m_firstLetterText->text();
                emitText(m_node, m_firstLetterText, m_offset, m_offset + firstLetter.length());
                m_firstLetterText = 0;
                m_textBox = 0;
                return false;
            }
        }
        if (renderer->style()->visibility() != VISIBLE && !m_ignoresStyleVisibility)
            return false;
        int strLength = str.length();
        int end = (m_node == m_endContainer) ? m_endOffset : INT_MAX;
        int runEnd = min(strLength, end);

        if (runStart >= runEnd)
            return true;

        emitText(m_node, runStart, runEnd);
        return true;
    }

    if (renderer->firstTextBox())
        m_textBox = renderer->firstTextBox();

    bool shouldHandleFirstLetter = !m_handledFirstLetter && renderer->isTextFragment() && !m_offset;
    if (shouldHandleFirstLetter)
        handleTextNodeFirstLetter(static_cast<RenderTextFragment*>(renderer));

    if (!renderer->firstTextBox() && str.length() > 0 && !shouldHandleFirstLetter) {
        if (renderer->style()->visibility() != VISIBLE && !m_ignoresStyleVisibility)
            return false;
        m_lastTextNodeEndedWithCollapsedSpace = true; // entire block is collapsed space
        return true;
    }

    if (m_firstLetterText)
        renderer = m_firstLetterText;

    if (renderer->containsReversedText()) {
        m_sortedTextBoxes.clear();
        for (InlineTextBox* textBox = renderer->firstTextBox(); textBox; textBox = textBox->nextTextBox()) {
            m_sortedTextBoxes.append(textBox);
        }
        std::sort(m_sortedTextBoxes.begin(), m_sortedTextBoxes.end(), InlineTextBox::compareByStart); 
        m_sortedTextBoxesPosition = 0;
        m_textBox = m_sortedTextBoxes.isEmpty() ? 0 : m_sortedTextBoxes[0];
    }

    handleTextBox();
    return true;
}
