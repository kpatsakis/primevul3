bool TextIterator::shouldRepresentNodeOffsetZero()
{
    if (m_emitsCharactersBetweenAllVisiblePositions && m_node->renderer() && m_node->renderer()->isTable())
        return true;
        
    if (m_lastCharacter == '\n')
        return false;
    
    if (m_hasEmitted)
        return true;
    

    if (m_node == m_startContainer)
        return false;
    
    if (!m_node->isDescendantOf(m_startContainer))
        return true;

    if (m_startOffset == 0)
        return false;
        
    if (!m_node->renderer() || m_node->renderer()->style()->visibility() != VISIBLE
        || (m_node->renderer()->isBlockFlow() && !toRenderBlock(m_node->renderer())->height() && !m_node->hasTagName(bodyTag)))
        return false;

    VisiblePosition startPos = VisiblePosition(Position(m_startContainer, m_startOffset, Position::PositionIsOffsetInAnchor), DOWNSTREAM);
    VisiblePosition currPos = VisiblePosition(positionBeforeNode(m_node), DOWNSTREAM);
    return startPos.isNotNull() && currPos.isNotNull() && !inSameLine(startPos, currPos);
}
