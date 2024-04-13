ShareableElementData::ShareableElementData(const Vector<Attribute>& attributes)
    : ElementData(attributes.size())
{
    for (unsigned i = 0; i < m_arraySize; ++i)
        new (&m_attributeArray[i]) Attribute(attributes[i]);
}
