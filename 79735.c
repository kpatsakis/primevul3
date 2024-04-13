void HTMLSelectElement::parseAttribute(const QualifiedName& name, const AtomicString& value)
{
    if (name == sizeAttr) {
        int oldSize = m_size;
        int size = value.toInt();
        AtomicString attrSize = AtomicString::number(size);
        if (attrSize != value) {
            if (Attribute* sizeAttribute = ensureUniqueElementData()->getAttributeItem(sizeAttr))
                sizeAttribute->setValue(attrSize);
        }
        size = max(size, 1);

        if (oldSize != size)
            updateListItemSelectedStates();

        m_size = size;
        setNeedsValidityCheck();
        if (m_size != oldSize && inActiveDocument()) {
            lazyReattachIfAttached();
            setRecalcListItems();
        }
    } else if (name == multipleAttr)
        parseMultipleAttribute(value);
    else if (name == accesskeyAttr) {
    } else
        HTMLFormControlElementWithState::parseAttribute(name, value);
}
