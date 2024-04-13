static void checkForSiblingStyleChanges(Element* e, RenderStyle* style, bool finishedParsingCallback,
                                        Node* beforeChange, Node* afterChange, int childCountDelta)
{
    checkForEmptyStyleChange(e, style);
    
    if (!style || (e->needsStyleRecalc() && e->childrenAffectedByPositionalRules()))
        return;

    if (e->childrenAffectedByFirstChildRules() && afterChange) {
        Node* newFirstChild = 0;
        for (newFirstChild = e->firstChild(); newFirstChild && !newFirstChild->isElementNode(); newFirstChild = newFirstChild->nextSibling()) {};
        
        Node* firstElementAfterInsertion = 0;
        for (firstElementAfterInsertion = afterChange;
             firstElementAfterInsertion && !firstElementAfterInsertion->isElementNode();
             firstElementAfterInsertion = firstElementAfterInsertion->nextSibling()) {};
        
        if (newFirstChild != firstElementAfterInsertion && firstElementAfterInsertion && firstElementAfterInsertion->attached() &&
            firstElementAfterInsertion->renderStyle() && firstElementAfterInsertion->renderStyle()->firstChildState())
            firstElementAfterInsertion->setNeedsStyleRecalc();
            
        if (childCountDelta < 0 && newFirstChild == firstElementAfterInsertion && newFirstChild && (!newFirstChild->renderStyle() || !newFirstChild->renderStyle()->firstChildState()))
            newFirstChild->setNeedsStyleRecalc();
    }

    if (e->childrenAffectedByLastChildRules() && beforeChange) {
        Node* newLastChild = 0;
        for (newLastChild = e->lastChild(); newLastChild && !newLastChild->isElementNode(); newLastChild = newLastChild->previousSibling()) {};
        
        Node* lastElementBeforeInsertion = 0;
        for (lastElementBeforeInsertion = beforeChange;
             lastElementBeforeInsertion && !lastElementBeforeInsertion->isElementNode();
             lastElementBeforeInsertion = lastElementBeforeInsertion->previousSibling()) {};
        
        if (newLastChild != lastElementBeforeInsertion && lastElementBeforeInsertion && lastElementBeforeInsertion->attached() &&
            lastElementBeforeInsertion->renderStyle() && lastElementBeforeInsertion->renderStyle()->lastChildState())
            lastElementBeforeInsertion->setNeedsStyleRecalc();
            
        if ((childCountDelta < 0 || finishedParsingCallback) && newLastChild == lastElementBeforeInsertion && newLastChild && (!newLastChild->renderStyle() || !newLastChild->renderStyle()->lastChildState()))
            newLastChild->setNeedsStyleRecalc();
    }

    if (e->childrenAffectedByDirectAdjacentRules() && afterChange) {
        Node* firstElementAfterInsertion = 0;
        for (firstElementAfterInsertion = afterChange;
             firstElementAfterInsertion && !firstElementAfterInsertion->isElementNode();
             firstElementAfterInsertion = firstElementAfterInsertion->nextSibling()) {};
        if (firstElementAfterInsertion && firstElementAfterInsertion->attached())
            firstElementAfterInsertion->setNeedsStyleRecalc();
    }

    if ((e->childrenAffectedByForwardPositionalRules() && afterChange)
        || (e->childrenAffectedByBackwardPositionalRules() && beforeChange))
        e->setNeedsStyleRecalc();
}
