void Element::normalizeAttributes()
{
    if (!hasAttributes())
        return;
    for (unsigned i = 0; i < attributeCount(); ++i) {
        if (RefPtr<Attr> attr = attrIfExists(attributeItem(i)->name()))
            attr->normalize();
    }
}
