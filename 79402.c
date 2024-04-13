ImageLoader::ImageLoader(Element* element)
    : m_element(element)
    , m_image(0)
    , m_derefElementTimer(this, &ImageLoader::timerFired)
    , m_hasPendingBeforeLoadEvent(false)
    , m_hasPendingLoadEvent(false)
    , m_hasPendingErrorEvent(false)
    , m_imageComplete(true)
    , m_loadManually(false)
    , m_elementIsProtected(false)
{
}
