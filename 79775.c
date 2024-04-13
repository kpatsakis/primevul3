void HTMLTextAreaElement::setValue(const String& value)
{
    setValueCommon(value);
    m_isDirty = true;
    setNeedsValidityCheck();
}
