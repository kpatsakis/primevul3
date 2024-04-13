void Element::removeAttributeNS(const AtomicString& namespaceURI, const AtomicString& localName)
{
    removeAttribute(QualifiedName(nullAtom, localName, namespaceURI));
}
