bool FrameLoader::didOpenURL(const KURL& url)
{
    if (m_frame->redirectScheduler()->redirectScheduledDuringLoad()) {
        return false;
    }

    m_frame->redirectScheduler()->cancel();
    m_frame->editor()->clearLastEditCommand();

    m_isComplete = false;
    m_isLoadingMainResource = true;
    m_didCallImplicitClose = false;

    if (!m_creatingInitialEmptyDocument) {
        m_frame->setJSStatusBarText(String());
        m_frame->setJSDefaultStatusBarText(String());
    }
    m_URL = url;
    if (m_URL.protocolInHTTPFamily() && !m_URL.host().isEmpty() && m_URL.path().isEmpty())
        m_URL.setPath("/");
    m_workingURL = m_URL;

    started();

    return true;
}
