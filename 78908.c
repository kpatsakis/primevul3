PassRefPtr<UniqueElementData> ElementData::makeUniqueCopy() const
{
    if (isUnique())
        return adoptRef(new UniqueElementData(static_cast<const UniqueElementData&>(*this)));
    return adoptRef(new UniqueElementData(static_cast<const ShareableElementData&>(*this)));
}
