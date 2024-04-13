ImageLoader::~ImageLoader()
{
    if (m_image)
        m_image->removeClient(this);

    ASSERT(m_hasPendingBeforeLoadEvent || !beforeLoadEventSender().hasPendingEvents(this));
    if (m_hasPendingBeforeLoadEvent)
        beforeLoadEventSender().cancelEvent(this);

    ASSERT(m_hasPendingLoadEvent || !loadEventSender().hasPendingEvents(this));
    if (m_hasPendingLoadEvent)
        loadEventSender().cancelEvent(this);

    ASSERT(m_hasPendingErrorEvent || !errorEventSender().hasPendingEvents(this));
    if (m_hasPendingErrorEvent)
        errorEventSender().cancelEvent(this);

    if (m_elementIsProtected)
        m_element->deref();
}
