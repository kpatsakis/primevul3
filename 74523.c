void FrameLoader::checkCallImplicitClose()
{
    if (m_didCallImplicitClose || m_frame->document()->parsing())
        return;

    if (!allChildrenAreComplete())
        return; // still got a frame running -> too early

    m_didCallImplicitClose = true;
    m_wasUnloadEventEmitted = false;
    m_frame->document()->implicitClose();
}
