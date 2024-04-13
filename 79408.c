void ImageLoader::dispatchPendingErrorEvent()
{
    if (!m_hasPendingErrorEvent)
        return;
    m_hasPendingErrorEvent = false;
    if (element()->document()->attached())
        element()->dispatchEvent(Event::create(eventNames().errorEvent, false, false));

    updatedHasPendingEvent();
}
