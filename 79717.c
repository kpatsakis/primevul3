void HTMLKeygenElement::parseAttribute(const QualifiedName& name, const AtomicString& value)
{
    if (name == disabledAttr)
        shadowSelect()->setAttribute(name, value);

    HTMLFormControlElement::parseAttribute(name, value);
}
