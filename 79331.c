static Node* nextInPreOrderCrossingShadowBoundaries(Node* rangeEndContainer, int rangeEndOffset)
{
    if (!rangeEndContainer)
        return 0;
    if (rangeEndOffset >= 0 && !rangeEndContainer->offsetInCharacters()) {
        if (Node* next = rangeEndContainer->childNode(rangeEndOffset))
            return next;
    }
    for (Node* node = rangeEndContainer; node; node = node->parentOrShadowHostNode()) {
        if (Node* next = node->nextSibling())
            return next;
    }
    return 0;
}
