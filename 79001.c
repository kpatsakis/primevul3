static size_t sizeForShareableElementDataWithAttributeCount(unsigned count)
{
    return sizeof(ShareableElementData) + sizeof(Attribute) * count;
}
