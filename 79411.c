void ImageLoader::dispatchPendingLoadEvent()
{
    if (!m_hasPendingLoadEvent)
        return;
    if (!m_image)
        return;
    m_hasPendingLoadEvent = false;
    if (element()->document()->attached())
        dispatchLoadEvent();

    updatedHasPendingEvent();
}
