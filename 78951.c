void Element::removedFrom(ContainerNode* insertionPoint)
{
    bool wasInDocument = insertionPoint->document();

    if (Element* before = pseudoElement(BEFORE))
        before->removedFrom(insertionPoint);

    if (Element* after = pseudoElement(AFTER))
        after->removedFrom(insertionPoint);

    document()->removeFromTopLayer(this);
    if (containsFullScreenElement())
        setContainsFullScreenElementOnAncestorsCrossingFrameBoundaries(false);

    if (document()->page())
        document()->page()->pointerLockController()->elementRemoved(this);

    setSavedLayerScrollOffset(IntSize());

    if (insertionPoint->isInTreeScope() && treeScope() == document()) {
        const AtomicString& idValue = getIdAttribute();
        if (!idValue.isNull())
            updateId(insertionPoint->treeScope(), idValue, nullAtom);

        const AtomicString& nameValue = getNameAttribute();
        if (!nameValue.isNull())
            updateName(nameValue, nullAtom);

        if (hasTagName(labelTag)) {
            TreeScope* treeScope = insertionPoint->treeScope();
            if (treeScope->shouldCacheLabelsByForAttribute())
                updateLabel(treeScope, fastGetAttribute(forAttr), nullAtom);
        }
    }

    ContainerNode::removedFrom(insertionPoint);
    if (wasInDocument && hasPendingResources())
        document()->accessSVGExtensions()->removeElementFromPendingResources(this);
}
