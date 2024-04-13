bool SimplifiedBackwardsTextIterator::handleReplacedElement()
{
    unsigned index = m_node->nodeIndex();
    emitCharacter(',', m_node->parentNode(), index, index + 1);
    return true;
}
