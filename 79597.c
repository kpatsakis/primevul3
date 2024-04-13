bool HTMLButtonElement::isURLAttribute(const Attribute& attribute) const
{
    return attribute.name() == formactionAttr || HTMLFormControlElement::isURLAttribute(attribute);
}
