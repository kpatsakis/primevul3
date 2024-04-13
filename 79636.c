void HTMLFormControlElement::setNeedsWillValidateCheck()
{
    bool newWillValidate = recalcWillValidate();
    if (m_willValidateInitialized && m_willValidate == newWillValidate)
        return;
    m_willValidateInitialized = true;
    m_willValidate = newWillValidate;
    setNeedsValidityCheck();
    setNeedsStyleRecalc();
    if (!m_willValidate)
        hideVisibleValidationMessage();
}
