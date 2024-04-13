void FrameLoader::setOpener(Frame* opener)
{
    if (m_opener)
        m_opener->loader()->m_openedFrames.remove(m_frame);
    if (opener)
        opener->loader()->m_openedFrames.add(m_frame);
    m_opener = opener;

    if (m_frame->document()) {
        m_frame->document()->initSecurityContext();
        m_frame->domWindow()->setSecurityOrigin(m_frame->document()->securityOrigin());
    }
}
