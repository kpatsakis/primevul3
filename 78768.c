PassRefPtr<DateTimeChooserImpl> DateTimeChooserImpl::create(ChromeClientImpl* chromeClient, DateTimeChooserClient* client, const DateTimeChooserParameters& parameters)
{
    return adoptRef(new DateTimeChooserImpl(chromeClient, client, parameters));
}
