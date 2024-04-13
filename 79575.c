void RenderLayerCompositor::updateCompositingRequirementsState()
{
    if (!m_needsUpdateCompositingRequirementsState)
        return;

    TRACE_EVENT0("blink_rendering,comp-scroll", "RenderLayerCompositor::updateCompositingRequirementsState");

    m_needsUpdateCompositingRequirementsState = false;

    if (!rootRenderLayer() || !m_renderView->acceleratedCompositingForOverflowScrollEnabled())
        return;

    for (HashSet<RenderLayer*>::iterator it = m_outOfFlowPositionedLayers.begin(); it != m_outOfFlowPositionedLayers.end(); ++it)
        (*it)->updateHasUnclippedDescendant();

    const FrameView::ScrollableAreaSet* scrollableAreas = m_renderView->frameView()->scrollableAreas();
    if (!scrollableAreas)
        return;

    for (FrameView::ScrollableAreaSet::iterator it = scrollableAreas->begin(); it != scrollableAreas->end(); ++it)
        (*it)->updateNeedsCompositedScrolling();
}
