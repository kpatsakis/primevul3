bool SimplifiedBackwardsTextIterator::handleNonTextNode()
{    
    if (shouldEmitNewlineForNode(m_node, m_emitsOriginalText) || shouldEmitNewlineAfterNode(m_node) || shouldEmitTabBeforeNode(m_node)) {
        unsigned index = m_node->nodeIndex();
        emitCharacter('\n', m_node->parentNode(), index + 1, index + 1);
    }
    return true;
}
