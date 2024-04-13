void HTMLFormElement::getTextFieldValues(StringPairVector& fieldNamesAndValues) const
{
    ASSERT_ARG(fieldNamesAndValues, fieldNamesAndValues.isEmpty());

    fieldNamesAndValues.reserveCapacity(m_associatedElements.size());
    for (unsigned i = 0; i < m_associatedElements.size(); ++i) {
        FormAssociatedElement* control = m_associatedElements[i];
        HTMLElement* element = toHTMLElement(control);
        if (!element->hasTagName(inputTag))
            continue;

        HTMLInputElement* input = toHTMLInputElement(element);
        if (!input->isTextField())
            continue;

        fieldNamesAndValues.append(make_pair(input->name().string(), input->value()));
    }
}
