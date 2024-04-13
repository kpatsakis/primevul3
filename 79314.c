void TextIterator::handleTextBox()
{    
    RenderText* renderer = m_firstLetterText ? m_firstLetterText : toRenderText(m_node->renderer());
    if (renderer->style()->visibility() != VISIBLE && !m_ignoresStyleVisibility) {
        m_textBox = 0;
        return;
    }
    String str = renderer->text();
    unsigned start = m_offset;
    unsigned end = (m_node == m_endContainer) ? static_cast<unsigned>(m_endOffset) : UINT_MAX;
    while (m_textBox) {
        unsigned textBoxStart = m_textBox->start();
        unsigned runStart = max(textBoxStart, start);

        InlineTextBox* firstTextBox = renderer->containsReversedText() ? (m_sortedTextBoxes.isEmpty() ? 0 : m_sortedTextBoxes[0]) : renderer->firstTextBox();
        bool needSpace = m_lastTextNodeEndedWithCollapsedSpace
            || (m_textBox == firstTextBox && textBoxStart == runStart && runStart > 0);
        if (needSpace && !isCollapsibleWhitespace(m_lastCharacter) && m_lastCharacter) {
            if (m_lastTextNode == m_node && runStart > 0 && str[runStart - 1] == ' ') {
                unsigned spaceRunStart = runStart - 1;
                while (spaceRunStart > 0 && str[spaceRunStart - 1] == ' ')
                    --spaceRunStart;
                emitText(m_node, renderer, spaceRunStart, spaceRunStart + 1);
            } else
                emitCharacter(' ', m_node, 0, runStart, runStart);
            return;
        }
        unsigned textBoxEnd = textBoxStart + m_textBox->len();
        unsigned runEnd = min(textBoxEnd, end);
        
        InlineTextBox* nextTextBox = 0;
        if (renderer->containsReversedText()) {
            if (m_sortedTextBoxesPosition + 1 < m_sortedTextBoxes.size())
                nextTextBox = m_sortedTextBoxes[m_sortedTextBoxesPosition + 1];
        } else 
            nextTextBox = m_textBox->nextTextBox();
        ASSERT(!nextTextBox || nextTextBox->renderer() == renderer);

        if (runStart < runEnd) {
            if (str[runStart] == '\n') {
                emitCharacter(' ', m_node, 0, runStart, runStart + 1);
                m_offset = runStart + 1;
            } else {
                size_t subrunEnd = str.find('\n', runStart);
                if (subrunEnd == notFound || subrunEnd > runEnd)
                    subrunEnd = runEnd;
    
                m_offset = subrunEnd;
                emitText(m_node, renderer, runStart, subrunEnd);
            }

            if (static_cast<unsigned>(m_positionEndOffset) < textBoxEnd)
                return;

            unsigned nextRunStart = nextTextBox ? nextTextBox->start() : str.length();
            if (nextRunStart > runEnd)
                m_lastTextNodeEndedWithCollapsedSpace = true; // collapsed space between runs or at the end
            m_textBox = nextTextBox;
            if (renderer->containsReversedText())
                ++m_sortedTextBoxesPosition;
            return;
        }
        m_textBox = nextTextBox;
        if (renderer->containsReversedText())
            ++m_sortedTextBoxesPosition;
    }
    if (!m_textBox && m_remainingTextBox) {
        m_textBox = m_remainingTextBox;
        m_remainingTextBox = 0;
        m_firstLetterText = 0;
        m_offset = 0;
        handleTextBox();
    }
}
