void Element::setAttribute(const QualifiedName& name, const AtomicString& value)
{
    synchronizeAttribute(name);
    size_t index = elementData() ? elementData()->getAttributeItemIndex(name) : notFound;
    setAttributeInternal(index, name, value, NotInSynchronizationOfLazyAttribute);
}
