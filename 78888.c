bool Element::hasAttribute(const AtomicString& localName) const
{
    if (!elementData())
        return false;
    synchronizeAttribute(localName);
    return elementData()->getAttributeItem(shouldIgnoreAttributeCase(this) ? localName.lower() : localName, false);
}
