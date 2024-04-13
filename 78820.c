NamedNodeMap* Element::attributes() const
{
    ElementRareData* rareData = const_cast<Element*>(this)->ensureElementRareData();
    if (NamedNodeMap* attributeMap = rareData->attributeMap())
        return attributeMap;

    rareData->setAttributeMap(NamedNodeMap::create(const_cast<Element*>(this)));
    return rareData->attributeMap();
}
