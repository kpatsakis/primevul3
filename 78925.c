void Element::parserSetAttributes(const Vector<Attribute>& attributeVector)
{
    ASSERT(!inDocument());
    ASSERT(!parentNode());
    ASSERT(!m_elementData);

    if (attributeVector.isEmpty())
        return;

    if (document() && document()->sharedObjectPool())
        m_elementData = document()->sharedObjectPool()->cachedShareableElementDataWithAttributes(attributeVector);
    else
        m_elementData = ShareableElementData::createWithAttributes(attributeVector);

    for (unsigned i = 0; i < attributeVector.size(); ++i)
        attributeChangedFromParserOrByCloning(attributeVector[i].name(), attributeVector[i].value(), ModifiedDirectly);
}
