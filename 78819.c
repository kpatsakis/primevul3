Attribute* UniqueElementData::attributeItem(unsigned index)
{
    ASSERT_WITH_SECURITY_IMPLICATION(index < length());
    return &m_attributeVector.at(index);
}
