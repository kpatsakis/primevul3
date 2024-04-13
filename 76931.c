void EditorClientBlackBerry::textFieldDidEndEditing(Element* element)
{
    if (m_webPagePrivate->m_webSettings->isFormAutofillEnabled()) {
        if (HTMLInputElement* inputElement = element->toInputElement())
            m_webPagePrivate->m_autofillManager->textFieldDidEndEditing(inputElement);
    }
}
