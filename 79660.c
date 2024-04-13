const AtomicString& HTMLInputElement::defaultValue() const
{
    return fastGetAttribute(valueAttr);
}
