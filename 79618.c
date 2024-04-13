void HTMLFormControlElement::hideVisibleValidationMessage()
{
    if (m_validationMessage)
        m_validationMessage->requestToHideMessage();
}
