void DateTimeChooserImpl::setValue(const String& value)
{
    m_client->didChooseValue(value);
}
