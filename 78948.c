void Element::removeAttributeInternal(size_t index, SynchronizationOfLazyAttribute inSynchronizationOfLazyAttribute)
{
    ASSERT_WITH_SECURITY_IMPLICATION(index < attributeCount());

    UniqueElementData* elementData = ensureUniqueElementData();

    QualifiedName name = elementData->attributeItem(index)->name();
    AtomicString valueBeingRemoved = elementData->attributeItem(index)->value();

    if (!inSynchronizationOfLazyAttribute) {
        if (!valueBeingRemoved.isNull())
            willModifyAttribute(name, valueBeingRemoved, nullAtom);
    }

    if (RefPtr<Attr> attrNode = attrIfExists(name))
        detachAttrNodeFromElementWithValue(attrNode.get(), elementData->attributeItem(index)->value());

    elementData->removeAttribute(index);

    if (!inSynchronizationOfLazyAttribute)
        didRemoveAttribute(name);
}
