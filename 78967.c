void Element::setAttribute(const AtomicString& localName, const AtomicString& value, ExceptionCode& ec)
{
    if (!Document::isValidName(localName)) {
        ec = INVALID_CHARACTER_ERR;
        return;
    }

    synchronizeAttribute(localName);
    const AtomicString& caseAdjustedLocalName = shouldIgnoreAttributeCase(this) ? localName.lower() : localName;

    size_t index = elementData() ? elementData()->getAttributeItemIndex(caseAdjustedLocalName, false) : notFound;
    const QualifiedName& qName = index != notFound ? attributeItem(index)->name() : QualifiedName(nullAtom, caseAdjustedLocalName, nullAtom);
    setAttributeInternal(index, qName, value, NotInSynchronizationOfLazyAttribute);
}
