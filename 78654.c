void FrameLoader::detachFromParent()
{
    RefPtr<Frame> protect(m_frame);

    closeURL();
    history()->saveScrollPositionAndViewStateToItem(history()->currentItem());
    detachChildren();
    stopAllLoaders();

    InspectorInstrumentation::frameDetachedFromParent(m_frame);

    if (m_documentLoader)
        m_documentLoader->detachFromFrame();
    m_documentLoader = 0;
    m_client->detachedFromParent();

    m_progressTracker.clear();

    if (Frame* parent = m_frame->tree()->parent()) {
        parent->loader()->closeAndRemoveChild(m_frame);
        parent->loader()->scheduleCheckCompleted();
    } else {
        m_frame->setView(0);
        m_frame->willDetachPage();
        m_frame->detachFromPage();
    }
}
