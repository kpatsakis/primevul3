void FrameLoader::pageHidden()
{
    m_unloadEventBeingDispatched = true;
    if (m_frame->domWindow())
        m_frame->domWindow()->dispatchEvent(PageTransitionEvent::create(EventNames().pagehideEvent, true), m_frame->document());
    m_unloadEventBeingDispatched = false;

    for (Frame* child = m_frame->tree()->firstChild(); child; child = child->tree()->nextSibling())
        child->loader()->pageHidden();
}
