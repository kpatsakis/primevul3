size_t ElementData::getAttrIndex(Attr* attr) const
{
    for (unsigned i = 0; i < length(); ++i) {
        if (attributeItem(i)->name() == attr->qualifiedName())
            return i;
    }
    return notFound;
}
