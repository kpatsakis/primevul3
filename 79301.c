void SimplifiedBackwardsTextIterator::exitNode()
{
    if (shouldEmitNewlineForNode(m_node, m_emitsOriginalText) || shouldEmitNewlineBeforeNode(m_node) || shouldEmitTabBeforeNode(m_node)) {
        emitCharacter('\n', m_node, 0, 0);
    }
}
