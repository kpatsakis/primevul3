void HTMLTextAreaElement::updateValue() const
{
    if (formControlValueMatchesRenderer())
        return;

    ASSERT(renderer());
    m_value = innerTextValue();
    const_cast<HTMLTextAreaElement*>(this)->setFormControlValueMatchesRenderer(true);
    const_cast<HTMLTextAreaElement*>(this)->notifyFormStateChanged();
    m_isDirty = true;
    const_cast<HTMLTextAreaElement*>(this)->updatePlaceholderVisibility(false);
}
