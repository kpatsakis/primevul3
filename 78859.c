PassRefPtr<Attr> Element::detachAttribute(size_t index)
{
    ASSERT(elementData());
    const Attribute* attribute = elementData()->attributeItem(index);
    RefPtr<Attr> attrNode = attrIfExists(attribute->name());
    if (attrNode)
        detachAttrNodeAtIndex(attrNode.get(), index);
    else {
        attrNode = Attr::create(document(), attribute->name(), attribute->value());
        removeAttributeInternal(index, NotInSynchronizationOfLazyAttribute);
    }
    return attrNode.release();
}
