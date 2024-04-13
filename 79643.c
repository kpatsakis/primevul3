void HTMLFormControlElement::updateVisibleValidationMessage()
{
    Page* page = document().page();
    if (!page)
        return;
    String message;
    if (renderer() && willValidate())
        message = validationMessage().stripWhiteSpace();
    if (!m_validationMessage)
        m_validationMessage = ValidationMessage::create(this);
    m_validationMessage->updateValidationMessage(message);
}
