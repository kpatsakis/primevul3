bool RenderLayerCompositor::isMainFrame() const
{
    return !m_renderView->document().ownerElement();
}
