inline void Element::setAttributeInternal(size_t index, const QualifiedName& name, const AtomicString& newValue, SynchronizationOfLazyAttribute inSynchronizationOfLazyAttribute)
{
    if (newValue.isNull()) {
        if (index != notFound)
            removeAttributeInternal(index, inSynchronizationOfLazyAttribute);
        return;
    }

    if (index == notFound) {
        addAttributeInternal(name, newValue, inSynchronizationOfLazyAttribute);
        return;
    }

    if (!inSynchronizationOfLazyAttribute)
        willModifyAttribute(name, attributeItem(index)->value(), newValue);

    if (newValue != attributeItem(index)->value()) {
        if (RefPtr<Attr> attrNode = inSynchronizationOfLazyAttribute ? 0 : attrIfExists(name))
            attrNode->setValue(newValue);
        else
            ensureUniqueElementData()->attributeItem(index)->setValue(newValue);
    }

    if (!inSynchronizationOfLazyAttribute)
        didModifyAttribute(name, newValue);
}
