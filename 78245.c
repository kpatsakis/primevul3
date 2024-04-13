bool HTMLFormElement::isURLAttribute(const Attribute& attribute) const
{
    return attribute.name() == actionAttr || HTMLElement::isURLAttribute(attribute);
}
