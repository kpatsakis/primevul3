const AtomicString& Element::getAttribute(const AtomicString& localName) const
{
    if (!elementData())
        return nullAtom;
    synchronizeAttribute(localName);
    if (const Attribute* attribute = elementData()->getAttributeItem(localName, shouldIgnoreAttributeCase(this)))
        return attribute->value();
    return nullAtom;
}
