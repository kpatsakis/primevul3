void HTMLFormControlElement::setNeedsValidityCheck()
{
    bool newIsValid = valid();
    if (willValidate() && newIsValid != m_isValid) {
        setNeedsStyleRecalc();
    }
    m_isValid = newIsValid;

    if (m_validationMessage && m_validationMessage->isVisible()) {
        updateVisibleValidationMessage();
    }
}
