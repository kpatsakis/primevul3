void HTMLFormControlElement::ancestorDisabledStateWasChanged()
{
    m_ancestorDisabledState = AncestorDisabledStateUnknown;
    disabledAttributeChanged();
}
