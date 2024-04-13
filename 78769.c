void DateTimeChooserImpl::didClosePopup()
{
    ASSERT(m_client);
    m_popup = 0;
    m_client->didEndChooser();
}
