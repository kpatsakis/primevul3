PassRefPtr<Attr> Element::removeAttributeNode(Attr* attr, ExceptionCode& ec)
{
    if (!attr) {
        ec = TYPE_MISMATCH_ERR;
        return 0;
    }
    if (attr->ownerElement() != this) {
        ec = NOT_FOUND_ERR;
        return 0;
    }

    ASSERT(document() == attr->document());

    synchronizeAttribute(attr->qualifiedName());

    size_t index = elementData()->getAttrIndex(attr);
    if (index == notFound) {
        ec = NOT_FOUND_ERR;
        return 0;
    }

    RefPtr<Attr> guard(attr);
    detachAttrNodeAtIndex(attr, index);
    return guard.release();
}
