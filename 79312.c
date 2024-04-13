bool TextIterator::handleReplacedElement()
{
    if (m_fullyClippedStack.top())
        return false;

    RenderObject* renderer = m_node->renderer();
    if (renderer->style()->visibility() != VISIBLE && !m_ignoresStyleVisibility)
        return false;

    if (m_lastTextNodeEndedWithCollapsedSpace) {
        emitCharacter(' ', m_lastTextNode->parentNode(), m_lastTextNode, 1, 1);
        return false;
    }

    if (m_entersTextControls && renderer->isTextControl()) {
        if (HTMLElement* innerTextElement = toRenderTextControl(renderer)->textFormControlElement()->innerTextElement()) {
            m_node = innerTextElement->containingShadowRoot();
            pushFullyClippedState(m_fullyClippedStack, m_node);
            m_offset = 0;
            return false;
        }
    }

    m_hasEmitted = true;

    if (m_emitsObjectReplacementCharacters && renderer && renderer->isReplaced()) {
        emitCharacter(objectReplacementCharacter, m_node->parentNode(), m_node, 0, 1);
        return true;
    }

    if (m_emitsCharactersBetweenAllVisiblePositions) {
        emitCharacter(',', m_node->parentNode(), m_node, 0, 1);
        return true;
    }

    m_positionNode = m_node->parentNode();
    m_positionOffsetBaseNode = m_node;
    m_positionStartOffset = 0;
    m_positionEndOffset = 1;
    m_textCharacters = 0;

    if (m_emitsImageAltText && renderer->isImage() && renderer->isRenderImage()) {
        m_text = toRenderImage(renderer)->altText();
        if (!m_text.isEmpty()) {
            m_textLength = m_text.length();
            m_lastCharacter = m_text[m_textLength - 1];
            return true;
        }
    }

    m_textLength = 0;
    m_lastCharacter = 0;

    return true;
}
