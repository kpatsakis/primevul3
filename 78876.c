const AtomicString& Element::getAttribute(const QualifiedName& name) const
{
    if (!elementData())
        return nullAtom;
    synchronizeAttribute(name);
    if (const Attribute* attribute = getAttributeItem(name))
        return attribute->value();
    return nullAtom;
}
