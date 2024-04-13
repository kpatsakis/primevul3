bool HTMLFormControlElement::isDisabledFormControl() const
{
    if (m_disabled)
        return true;

    if (m_ancestorDisabledState == AncestorDisabledStateUnknown)
        updateAncestorDisabledState();
    return m_ancestorDisabledState == AncestorDisabledStateDisabled;
}
