void HTMLInputElement::handleBlurEvent()
{
    m_inputType->disableSecureTextInput();
    m_inputTypeView->handleBlurEvent();
}
