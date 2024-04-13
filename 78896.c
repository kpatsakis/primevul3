Node::InsertionNotificationRequest Element::insertedInto(ContainerNode* insertionPoint)
{
    ContainerNode::insertedInto(insertionPoint);

    if (containsFullScreenElement() && parentElement() && !parentElement()->containsFullScreenElement())
        setContainsFullScreenElementOnAncestorsCrossingFrameBoundaries(true);

    if (Element* before = pseudoElement(BEFORE))
        before->insertedInto(insertionPoint);

    if (Element* after = pseudoElement(AFTER))
        after->insertedInto(insertionPoint);

    if (!insertionPoint->isInTreeScope())
        return InsertionDone;

    if (hasRareData())
        elementRareData()->clearClassListValueForQuirksMode();

    TreeScope* scope = insertionPoint->treeScope();
    if (scope != treeScope())
        return InsertionDone;

    const AtomicString& idValue = getIdAttribute();
    if (!idValue.isNull())
        updateId(scope, nullAtom, idValue);

    const AtomicString& nameValue = getNameAttribute();
    if (!nameValue.isNull())
        updateName(nullAtom, nameValue);

    if (hasTagName(labelTag)) {
        if (scope->shouldCacheLabelsByForAttribute())
            updateLabel(scope, nullAtom, fastGetAttribute(forAttr));
    }

    return InsertionDone;
}
