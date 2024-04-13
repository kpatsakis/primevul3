void Element::removeAttribute(const QualifiedName& name)
{
    if (!elementData())
        return;

    size_t index = elementData()->getAttributeItemIndex(name);
    if (index == notFound)
        return;

    removeAttributeInternal(index, NotInSynchronizationOfLazyAttribute);
}
