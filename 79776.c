void HTMLTextAreaElement::setValueCommon(const String& newValue)
{
    String normalizedValue = newValue.isNull() ? "" : newValue;
    normalizedValue.replace("\r\n", "\n");
    normalizedValue.replace('\r', '\n');

    if (normalizedValue == value())
        return;

    m_value = normalizedValue;
    setInnerTextValue(m_value);
    setLastChangeWasNotUserEdit();
    updatePlaceholderVisibility(false);
    setNeedsStyleRecalc();
    setFormControlValueMatchesRenderer(true);

    if (document().focusedElement() == this) {
        unsigned endOfString = m_value.length();
        setSelectionRange(endOfString, endOfString);
    }

    notifyFormStateChanged();
    setTextAsOfLastFormControlChangeEvent(normalizedValue);
}
