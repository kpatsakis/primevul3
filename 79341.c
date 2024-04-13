PassRefPtr<Range> SimplifiedBackwardsTextIterator::range() const
{
    if (m_positionNode)
        return Range::create(m_positionNode->document(), m_positionNode, m_positionStartOffset, m_positionNode, m_positionEndOffset);
    
    return Range::create(m_startNode->document(), m_startNode, m_startOffset, m_startNode, m_startOffset);
}
