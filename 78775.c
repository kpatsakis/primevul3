void DateTimeChooserImpl::setValueAndClosePopup(int numValue, const String& stringValue)
{
    RefPtr<DateTimeChooserImpl> protector(this);
    if (numValue >= 0)
        setValue(stringValue);
    endChooser();
}
