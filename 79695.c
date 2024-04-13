void HTMLInputElement::setType(const AtomicString& type)
{
    if (type.isEmpty())
        removeAttribute(typeAttr);
    else
        setAttribute(typeAttr, type);
}
