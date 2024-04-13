bool SimplifiedBackwardsTextIterator::advanceRespectingRange(Node* next)
{
    if (!next)
        return false;
    m_havePassedStartNode |= m_node == m_startNode;
    if (m_havePassedStartNode)
        return false;
    m_node = next;
    return true;
}
