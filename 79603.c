bool HTMLButtonElement::willRespondToMouseClickEvents()
{
    if (!isDisabledFormControl() && form() && (m_type == SUBMIT || m_type == RESET))
        return true;
    return HTMLFormControlElement::willRespondToMouseClickEvents();
}
