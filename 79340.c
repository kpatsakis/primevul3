PassRefPtr<Range> TextIterator::range() const
{
    if (m_positionNode) {
        if (m_positionOffsetBaseNode) {
            int index = m_positionOffsetBaseNode->nodeIndex();
            m_positionStartOffset += index;
            m_positionEndOffset += index;
            m_positionOffsetBaseNode = 0;
        }
        return Range::create(m_positionNode->document(), m_positionNode, m_positionStartOffset, m_positionNode, m_positionEndOffset);
    }

    if (m_endContainer)
        return Range::create(m_endContainer->document(), m_endContainer, m_endOffset, m_endContainer, m_endOffset);
        
    return 0;
}
