void Element::setSynchronizedLazyAttribute(const QualifiedName& name, const AtomicString& value)
{
    size_t index = elementData() ? elementData()->getAttributeItemIndex(name) : notFound;
    setAttributeInternal(index, name, value, InSynchronizationOfLazyAttribute);
}
