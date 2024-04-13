bool RenderLayerCompositor::requiresCompositingForScrollableFrame() const
{
    ASSERT(!m_renderView->needsLayout());
    if (isMainFrame())
        return false;

    if (!(m_compositingTriggers & ChromeClient::ScrollableInnerFrameTrigger))
        return false;

    FrameView* frameView = m_renderView->frameView();
    return frameView->isScrollable();
}
