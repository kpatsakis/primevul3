bool HTMLButtonElement::isPresentationAttribute(const QualifiedName& name) const
{
    if (name == alignAttr) {
        return false;
    }

    return HTMLFormControlElement::isPresentationAttribute(name);
}
