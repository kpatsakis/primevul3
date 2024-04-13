void TextIterator::advance()
{
    if (m_shouldStop)
        return;

    m_positionNode = 0;
    m_textLength = 0;

    if (m_needsAnotherNewline) {
        Node* baseNode = m_node->lastChild() ? m_node->lastChild() : m_node;
        emitCharacter('\n', baseNode->parentNode(), baseNode, 1, 1);
        m_needsAnotherNewline = false;
        return;
    }

    if (!m_textBox && m_remainingTextBox) {
        m_textBox = m_remainingTextBox;
        m_remainingTextBox = 0;
        m_firstLetterText = 0;
        m_offset = 0;
    }
    if (m_textBox) {
        handleTextBox();
        if (m_positionNode)
            return;
    }

    while (m_node && m_node != m_pastEndNode) {
        if (!m_shouldStop && m_stopsOnFormControls && HTMLFormControlElement::enclosingFormControlElement(m_node))
            m_shouldStop = true;

        if (m_node == m_endContainer && m_endOffset == 0) {
            representNodeOffsetZero();
            m_node = 0;
            return;
        }
        
        RenderObject* renderer = m_node->renderer();
        if (!renderer) {
            m_handledNode = true;
            m_handledChildren = true;
        } else {
            if (!m_handledNode) {
                if (renderer->isText() && m_node->nodeType() == Node::TEXT_NODE) // FIXME: What about CDATA_SECTION_NODE?
                    m_handledNode = handleTextNode();
                else if (renderer && (renderer->isImage() || renderer->isWidget() ||
                         (renderer->node() && renderer->node()->isElementNode() &&
                          (toElement(renderer->node())->isFormControlElement()
                          || toElement(renderer->node())->hasTagName(legendTag)
                          || toElement(renderer->node())->hasTagName(meterTag)
                          || toElement(renderer->node())->hasTagName(progressTag)))))
                    m_handledNode = handleReplacedElement();
                else
                    m_handledNode = handleNonTextNode();
                if (m_positionNode)
                    return;
            }
        }

        Node* next = m_handledChildren ? 0 : m_node->firstChild();
        m_offset = 0;
        if (!next) {
            next = m_node->nextSibling();
            if (!next) {
                bool pastEnd = NodeTraversal::next(m_node) == m_pastEndNode;
                Node* parentNode = m_node->parentOrShadowHostNode();
                while (!next && parentNode) {
                    if ((pastEnd && parentNode == m_endContainer) || m_endContainer->isDescendantOf(parentNode))
                        return;
                    bool haveRenderer = m_node->renderer();
                    m_node = parentNode;
                    m_fullyClippedStack.pop();
                    parentNode = m_node->parentOrShadowHostNode();
                    if (haveRenderer)
                        exitNode();
                    if (m_positionNode) {
                        m_handledNode = true;
                        m_handledChildren = true;
                        return;
                    }
                    next = m_node->nextSibling();
                }
            }
            m_fullyClippedStack.pop();            
        }

        m_node = next;
        if (m_node)
            pushFullyClippedState(m_fullyClippedStack, m_node);
        m_handledNode = false;
        m_handledChildren = false;
        m_handledFirstLetter = false;
        m_firstLetterText = 0;

        if (m_positionNode)
            return;
    }
}
