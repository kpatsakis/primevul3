void Element::addAttributeInternal(const QualifiedName& name, const AtomicString& value, SynchronizationOfLazyAttribute inSynchronizationOfLazyAttribute)
{
    if (!inSynchronizationOfLazyAttribute)
        willModifyAttribute(name, nullAtom, value);
    ensureUniqueElementData()->addAttribute(name, value);
    if (!inSynchronizationOfLazyAttribute)
        didAddAttribute(name, value);
}
