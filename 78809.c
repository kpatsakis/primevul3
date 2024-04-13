void UniqueElementData::addAttribute(const QualifiedName& attributeName, const AtomicString& value)
{
    m_attributeVector.append(Attribute(attributeName, value));
}
