PassRefPtr<ShareableElementData> ShareableElementData::createWithAttributes(const Vector<Attribute>& attributes)
{
    void* slot = WTF::fastMalloc(sizeForShareableElementDataWithAttributeCount(attributes.size()));
    return adoptRef(new (slot) ShareableElementData(attributes));
}
