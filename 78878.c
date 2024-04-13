Attribute* UniqueElementData::getAttributeItem(const QualifiedName& name)
{
    for (unsigned i = 0; i < length(); ++i) {
        if (m_attributeVector.at(i).name().matches(name))
            return &m_attributeVector.at(i);
    }
    return 0;
}
