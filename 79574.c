void RenderLayerCompositor::updateCompositingLayers()
{
    TRACE_EVENT0("blink_rendering", "RenderLayerCompositor::updateCompositingLayers");


    if (m_renderView->needsLayout())
        return;

    lifecycle().advanceTo(DocumentLifecycle::InCompositingUpdate);
    DocumentLifecycle::Scope lifecycleScope(lifecycle(), DocumentLifecycle::CompositingClean);

    if (isMainFrame() && m_renderView->frameView())
        finishCompositingUpdateForFrameTree(&m_renderView->frameView()->frame());

    if (m_forceCompositingMode && !m_compositing)
        enableCompositingMode(true);

    if (!m_needsToRecomputeCompositingRequirements && !m_compositing)
        return;

    TemporaryChange<bool> postLayoutChange(m_inPostLayoutUpdate, true);

    bool needCompositingRequirementsUpdate = m_needsToRecomputeCompositingRequirements;
    bool needHierarchyAndGeometryUpdate = m_compositingLayersNeedRebuild;
    bool needGeometryUpdate = m_needsToUpdateLayerTreeGeometry;
    bool needsToUpdateScrollingCoordinator = scrollingCoordinator() ? scrollingCoordinator()->needsToUpdateAfterCompositingChange() : false;

    if (!needCompositingRequirementsUpdate && !needHierarchyAndGeometryUpdate && !needGeometryUpdate && !needsToUpdateScrollingCoordinator)
        return;

    m_compositingLayersNeedRebuild = false;
    m_needsToUpdateLayerTreeGeometry = false;
    m_needsToRecomputeCompositingRequirements = false;
    RenderLayer* updateRoot = rootRenderLayer();

    if (needCompositingRequirementsUpdate) {
        CompositingRecursionData recursionData(updateRoot, 0, true);
        bool layersChanged = false;
        bool saw3DTransform = false;
        {
            TRACE_EVENT0("blink_rendering", "RenderLayerCompositor::computeCompositingRequirements");
            OverlapMap overlapTestRequestMap;

            Vector<RenderLayer*> unclippedDescendants;
            computeCompositingRequirements(0, updateRoot, &overlapTestRequestMap, recursionData, saw3DTransform, unclippedDescendants);
        }

        {
            TRACE_EVENT0("blink_rendering", "RenderLayerCompositor::assignLayersToBackings");
            assignLayersToBackings(updateRoot, layersChanged);
        }

        {
            TRACE_EVENT0("blink_rendering", "RenderLayerCompositor::updateHasVisibleNonLayerContentLoop");
            const FrameView::ScrollableAreaSet* scrollableAreas = m_renderView->frameView()->scrollableAreas();
            if (scrollableAreas) {
                for (FrameView::ScrollableAreaSet::iterator it = scrollableAreas->begin(); it != scrollableAreas->end(); ++it)
                    (*it)->updateHasVisibleNonLayerContent();
            }
        }

        needHierarchyAndGeometryUpdate |= layersChanged;
    }

    if (needHierarchyAndGeometryUpdate) {
        Vector<GraphicsLayer*> childList;
        {
            TRACE_EVENT0("blink_rendering", "RenderLayerCompositor::rebuildCompositingLayerTree");
            rebuildCompositingLayerTree(updateRoot, childList, 0);
        }

        if (RuntimeEnabledFeatures::overlayFullscreenVideoEnabled() && isMainFrame()) {
            RenderVideo* video = findFullscreenVideoRenderer(&m_renderView->document());
            if (video && video->hasCompositedLayerMapping()) {
                childList.clear();
                childList.append(video->compositedLayerMapping()->mainGraphicsLayer());
            }
        }

        if (childList.isEmpty())
            destroyRootLayer();
        else
            m_rootContentLayer->setChildren(childList);
    } else if (needGeometryUpdate) {
        TRACE_EVENT0("blink_rendering", "RenderLayerCompositor::updateLayerTreeGeometry");
        updateLayerTreeGeometry(updateRoot);
    }

    ASSERT(updateRoot || !m_compositingLayersNeedRebuild);

    if (!hasAcceleratedCompositing())
        enableCompositingMode(false);

    needsToUpdateScrollingCoordinator |= scrollingCoordinator() ? scrollingCoordinator()->needsToUpdateAfterCompositingChange() : false;
    if (needsToUpdateScrollingCoordinator && isMainFrame() && scrollingCoordinator() && inCompositingMode())
        scrollingCoordinator()->updateAfterCompositingChange();

    if (isMainFrame())
        InspectorInstrumentation::layerTreeDidChange(page());
}
