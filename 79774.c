void HTMLTextAreaElement::setSuggestedValue(const String& value)
{
    m_suggestedValue = value;
    setInnerTextValue(m_suggestedValue);
    updatePlaceholderVisibility(false);
    setNeedsStyleRecalc();
    setFormControlValueMatchesRenderer(true);
}
