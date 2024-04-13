void UniqueElementData::removeAttribute(size_t index)
{
    ASSERT_WITH_SECURITY_IMPLICATION(index < length());
    m_attributeVector.remove(index);
}
