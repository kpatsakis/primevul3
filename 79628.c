void HTMLFormControlElement::reset()
{
    setAutofilled(false);
    resetImpl();
}
