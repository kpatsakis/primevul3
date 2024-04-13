PassRefPtr<Attr> Element::getAttributeNode(const AtomicString& localName)
{
    if (!elementData())
        return 0;
    synchronizeAttribute(localName);
    const Attribute* attribute = elementData()->getAttributeItem(localName, shouldIgnoreAttributeCase(this));
    if (!attribute)
        return 0;
    return ensureAttr(attribute->name());
}
