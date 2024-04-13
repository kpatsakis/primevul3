void ImageLoader::updatedHasPendingEvent()
{
    bool wasProtected = m_elementIsProtected;
    m_elementIsProtected = m_hasPendingLoadEvent || m_hasPendingErrorEvent;
    if (wasProtected == m_elementIsProtected)
        return;

    if (m_elementIsProtected) {
        if (m_derefElementTimer.isActive())
            m_derefElementTimer.stop();
        else
            m_element->ref();
    } else {
        ASSERT(!m_derefElementTimer.isActive());
        m_derefElementTimer.startOneShot(0);
    }   
}
