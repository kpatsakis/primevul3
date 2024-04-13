void SimplifiedBackwardsTextIterator::advance()
{
    ASSERT(m_positionNode);

    if (m_shouldStop)
        return;

    if (m_stopsOnFormControls && HTMLFormControlElement::enclosingFormControlElement(m_node)) {
        m_shouldStop = true;
        return;
    }

    m_positionNode = 0;
    m_textLength = 0;

    while (m_node && !m_havePassedStartNode) {
        if (!m_handledNode && !(m_node == m_endNode && m_endOffset == 0)) {
            RenderObject* renderer = m_node->renderer();
            if (renderer && renderer->isText() && m_node->nodeType() == Node::TEXT_NODE) {
                if (renderer->style()->visibility() == VISIBLE && m_offset > 0)
                    m_handledNode = handleTextNode();
            } else if (renderer && (renderer->isImage() || renderer->isWidget())) {
                if (renderer->style()->visibility() == VISIBLE && m_offset > 0)
                    m_handledNode = handleReplacedElement();
            } else
                m_handledNode = handleNonTextNode();
            if (m_positionNode)
                return;
        }

        if (!m_handledChildren && m_node->hasChildNodes()) {
            m_node = m_node->lastChild();
            pushFullyClippedState(m_fullyClippedStack, m_node);
        } else {
            if (!m_handledNode
                    && canHaveChildrenForEditing(m_node)
                    && m_node->parentNode()
                    && (!m_node->lastChild() || (m_node == m_endNode && !m_endOffset))) {
                exitNode();
                if (m_positionNode) {
                    m_handledNode = true;
                    m_handledChildren = true;
                    return;
                }
            }

            while (!m_node->previousSibling()) {
                if (!advanceRespectingRange(m_node->parentOrShadowHostNode()))
                    break;
                m_fullyClippedStack.pop();
                exitNode();
                if (m_positionNode) {
                    m_handledNode = true;
                    m_handledChildren = true;
                    return;
                }
            }

            m_fullyClippedStack.pop();
            if (advanceRespectingRange(m_node->previousSibling()))
                pushFullyClippedState(m_fullyClippedStack, m_node);
            else
                m_node = 0;
        }

        m_offset = m_node ? maxOffsetIncludingCollapsedSpaces(m_node) : 0;
        m_handledNode = false;
        m_handledChildren = false;
        
        if (m_positionNode)
            return;
    }
}
