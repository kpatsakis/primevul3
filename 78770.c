void DateTimeChooserImpl::endChooser()
{
    if (!m_popup)
        return;
    m_chromeClient->closePagePopup(m_popup);
}
