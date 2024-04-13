void HTMLInputElement::handleFocusEvent(Element* oldFocusedElement, FocusDirection direction)
{
    m_inputTypeView->handleFocusEvent(oldFocusedElement, direction);
    m_inputType->enableSecureTextInput();
}
