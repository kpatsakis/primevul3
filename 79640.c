bool HTMLFormControlElement::supportsFocus() const
{
    return !isDisabledFormControl();
}
