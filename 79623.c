bool HTMLFormControlElement::isSuccessfulSubmitButton() const
{
    return canBeSuccessfulSubmitButton() && !isDisabledFormControl();
}
