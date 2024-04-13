void HTMLInputElement::setValue(const String& value, TextFieldEventBehavior eventBehavior)
{
    if (!m_inputType->canSetValue(value))
        return;

    RefPtr<HTMLInputElement> protector(this);
    EventQueueScope scope;
    String sanitizedValue = sanitizeValue(value);
    bool valueChanged = sanitizedValue != this->value();

    setLastChangeWasNotUserEdit();
    setFormControlValueMatchesRenderer(false);
    m_suggestedValue = String(); // Prevent TextFieldInputType::setValue from using the suggested value.

    m_inputType->setValue(sanitizedValue, valueChanged, eventBehavior);

    if (valueChanged && eventBehavior == DispatchNoEvent)
        setTextAsOfLastFormControlChangeEvent(sanitizedValue);

    if (!valueChanged)
        return;

    notifyFormStateChanged();
}
