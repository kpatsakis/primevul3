ShareableElementData::~ShareableElementData()
{
    for (unsigned i = 0; i < m_arraySize; ++i)
        m_attributeArray[i].~Attribute();
}
