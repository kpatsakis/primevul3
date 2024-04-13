bool HTMLFormElement::shouldAutocomplete() const
{
    return !equalIgnoringCase(fastGetAttribute(autocompleteAttr), "off");
}
