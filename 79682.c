void HTMLInputElement::setDefaultValue(const AtomicString& value)
{
    setAttribute(valueAttr, value);
}
