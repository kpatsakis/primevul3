void HTMLTextAreaElement::setNonDirtyValue(const String& value)
{
    setValueCommon(value);
    m_isDirty = false;
    setNeedsValidityCheck();
}
