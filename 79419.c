void ImageLoader::setImageWithoutConsideringPendingLoadEvent(CachedImage* newImage)
{
    ASSERT(m_failedLoadURL.isEmpty());
    CachedImage* oldImage = m_image.get();
    if (newImage != oldImage) {
        m_image = newImage;
        if (m_hasPendingBeforeLoadEvent) {
            beforeLoadEventSender().cancelEvent(this);
            m_hasPendingBeforeLoadEvent = false;
        }
        if (m_hasPendingLoadEvent) {
            loadEventSender().cancelEvent(this);
            m_hasPendingLoadEvent = false;
        }
        if (m_hasPendingErrorEvent) {
            errorEventSender().cancelEvent(this);
            m_hasPendingErrorEvent = false;
        }
        m_imageComplete = true;
        if (newImage)
            newImage->addClient(this);
        if (oldImage)
            oldImage->removeClient(this);
    }

    if (RenderImageResource* imageResource = renderImageResource())
        imageResource->resetAnimation();
}
