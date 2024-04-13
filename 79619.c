bool HTMLFormControlElement::isDefaultButtonForForm() const
{
    return isSuccessfulSubmitButton() && form() && form()->defaultButton() == this;
}
