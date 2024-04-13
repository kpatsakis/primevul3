void ImageLoader::notifyFinished(CachedResource* resource)
{
    ASSERT(m_failedLoadURL.isEmpty());
    ASSERT(resource == m_image.get());

    m_imageComplete = true;
    if (!hasPendingBeforeLoadEvent())
        updateRenderer();

    if (!m_hasPendingLoadEvent)
        return;

    if (m_element->fastHasAttribute(HTMLNames::crossoriginAttr)
        && !m_element->document()->securityOrigin()->canRequest(image()->response().url())
        && !resource->passesAccessControlCheck(m_element->document()->securityOrigin())) {

        setImageWithoutConsideringPendingLoadEvent(0);

        m_hasPendingErrorEvent = true;
        errorEventSender().dispatchEventSoon(this);

        DEFINE_STATIC_LOCAL(String, consoleMessage, (ASCIILiteral("Cross-origin image load denied by Cross-Origin Resource Sharing policy.")));
        m_element->document()->addConsoleMessage(SecurityMessageSource, ErrorMessageLevel, consoleMessage);

        ASSERT(!m_hasPendingLoadEvent);

        updatedHasPendingEvent();
        return;
    }

    if (resource->wasCanceled()) {
        m_hasPendingLoadEvent = false;
        updatedHasPendingEvent();
        return;
    }

    loadEventSender().dispatchEventSoon(this);
}
