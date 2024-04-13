void RenderLayerCompositor::setNeedsCompositingUpdate(CompositingUpdateType updateType)
{

    if (m_renderView->needsLayout())
        return;

    if (m_forceCompositingMode && !m_compositing)
        enableCompositingMode(true);

    if (!m_needsToRecomputeCompositingRequirements && !m_compositing)
        return;

    switch (updateType) {
    case CompositingUpdateAfterStyleChange:
    case CompositingUpdateAfterLayout:
        m_needsToRecomputeCompositingRequirements = true;
        break;
    case CompositingUpdateOnScroll:
        m_needsToRecomputeCompositingRequirements = true; // Overlap can change with scrolling, so need to check for hierarchy updates.
        m_needsToUpdateLayerTreeGeometry = true;
        break;
    case CompositingUpdateOnCompositedScroll:
        m_needsToUpdateLayerTreeGeometry = true;
        break;
    }

    m_renderView->frameView()->scheduleAnimation();
}
