void ImageLoader::dispatchPendingBeforeLoadEvent()
{
    if (!m_hasPendingBeforeLoadEvent)
        return;
    if (!m_image)
        return;
    if (!m_element->document()->attached())
        return;
    m_hasPendingBeforeLoadEvent = false;
    if (m_element->dispatchBeforeLoadEvent(m_image->url())) {
        updateRenderer();
        return;
    }
    if (m_image) {
        m_image->removeClient(this);
        m_image = 0;
    }

    loadEventSender().cancelEvent(this);
    m_hasPendingLoadEvent = false;
    
    if (m_element->hasTagName(HTMLNames::objectTag))
        static_cast<HTMLObjectElement*>(m_element)->renderFallbackContent();

    updatedHasPendingEvent();
}
