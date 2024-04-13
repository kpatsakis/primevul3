void HTMLInputElement::setSuggestedValue(const String& value)
{
    if (!m_inputType->canSetSuggestedValue())
        return;
    setFormControlValueMatchesRenderer(false);
    m_suggestedValue = sanitizeValue(value);
    setNeedsStyleRecalc();
    m_inputTypeView->updateView();
}
