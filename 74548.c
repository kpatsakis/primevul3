void FrameLoader::detachFromParent()
{
    RefPtr<Frame> protect(m_frame);

    closeURL();
    stopAllLoaders();
    history()->saveScrollPositionAndViewStateToItem(history()->currentItem());
    detachChildren();

#if ENABLE(INSPECTOR)
    if (Page* page = m_frame->page())
        page->inspectorController()->frameDetachedFromParent(m_frame);
#endif

    detachViewsAndDocumentLoader();

    if (Frame* parent = m_frame->tree()->parent()) {
        parent->loader()->closeAndRemoveChild(m_frame);
        parent->loader()->scheduleCheckCompleted();
    } else {
        m_frame->setView(0);
        m_frame->pageDestroyed();
    }
}
