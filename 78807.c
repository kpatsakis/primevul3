UniqueElementData::UniqueElementData(const ShareableElementData& other)
    : ElementData(other, true)
{
    ASSERT(!other.m_inlineStyle || !other.m_inlineStyle->isMutable());
    m_inlineStyle = other.m_inlineStyle;

    m_attributeVector.reserveCapacity(other.length());
    for (unsigned i = 0; i < other.length(); ++i)
        m_attributeVector.uncheckedAppend(other.m_attributeArray[i]);
}
