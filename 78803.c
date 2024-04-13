ShareableElementData::ShareableElementData(const UniqueElementData& other)
    : ElementData(other, false)
{
    ASSERT(!other.m_presentationAttributeStyle);

    if (other.m_inlineStyle) {
        ASSERT(!other.m_inlineStyle->hasCSSOMWrapper());
        m_inlineStyle = other.m_inlineStyle->immutableCopyIfNeeded();
    }

    for (unsigned i = 0; i < m_arraySize; ++i)
        new (&m_attributeArray[i]) Attribute(other.m_attributeVector.at(i));
}
