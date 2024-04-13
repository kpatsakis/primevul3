void Element::removeAttribute(const AtomicString& name)
{
    if (!elementData())
        return;

    AtomicString localName = shouldIgnoreAttributeCase(this) ? name.lower() : name;
    size_t index = elementData()->getAttributeItemIndex(localName, false);
    if (index == notFound) {
        if (UNLIKELY(localName == styleAttr) && elementData()->m_styleAttributeIsDirty && isStyledElement())
            static_cast<StyledElement*>(this)->removeAllInlineStyleProperties();
        return;
    }

    removeAttributeInternal(index, NotInSynchronizationOfLazyAttribute);
}
