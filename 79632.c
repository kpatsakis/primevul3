void HTMLFormControlElement::setCustomValidity(const String& error)
{
    FormAssociatedElement::setCustomValidity(error);
    setNeedsValidityCheck();
}
