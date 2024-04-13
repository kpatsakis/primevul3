bool TextIterator::shouldEmitSpaceBeforeAndAfterNode(Node* node)
{
    return node->renderer() && node->renderer()->isTable() && (node->renderer()->isInline() || m_emitsCharactersBetweenAllVisiblePositions);
}
