HTMLFormControlElement* HTMLFormElement::defaultButton() const
{
    for (unsigned i = 0; i < m_associatedElements.size(); ++i) {
        if (!m_associatedElements[i]->isFormControlElement())
            continue;
        HTMLFormControlElement* control = toHTMLFormControlElement(m_associatedElements[i]);
        if (control->isSuccessfulSubmitButton())
            return control;
    }

    return 0;
}
