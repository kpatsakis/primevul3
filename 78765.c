DateTimeChooserImpl::DateTimeChooserImpl(ChromeClientImpl* chromeClient, DateTimeChooserClient* client, const DateTimeChooserParameters& parameters)
    : m_chromeClient(chromeClient)
    , m_client(client)
    , m_popup(0)
    , m_parameters(parameters)
    , m_locale(Locale::create(parameters.locale))
{
    ASSERT(m_chromeClient);
    ASSERT(m_client);
    m_popup = m_chromeClient->openPagePopup(this, m_parameters.anchorRectInRootView);
}
