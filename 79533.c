void RenderLayerCompositor::rebuildCompositingLayerTree(RenderLayer* layer, Vector<GraphicsLayer*>& childLayersOfEnclosingLayer, int depth)
{

    layer->stackingNode()->updateLayerListsIfNeeded();
    layer->update3dRenderingContext();

    static double pixelsWithoutPromotingAllTransitions = 0.0;
    static double pixelsAddedByPromotingAllTransitions = 0.0;

    if (!depth) {
        pixelsWithoutPromotingAllTransitions = 0.0;
        pixelsAddedByPromotingAllTransitions = 0.0;
    }

    const bool hasCompositedLayerMapping = layer->hasCompositedLayerMapping();
    CompositedLayerMappingPtr currentCompositedLayerMapping = layer->compositedLayerMapping();

    updateGraphicsLayersMappedToRenderLayer(layer);

    if (hasCompositedLayerMapping) {
        pixelsWithoutPromotingAllTransitions += layer->size().height() * layer->size().width();
    } else {
        if ((layer->renderer()->style()->transitionForProperty(CSSPropertyOpacity) ||
             layer->renderer()->style()->transitionForProperty(CSSPropertyWebkitTransform)) &&
            m_renderView->viewRect().intersects(layer->absoluteBoundingBox()))
            pixelsAddedByPromotingAllTransitions += layer->size().height() * layer->size().width();
    }

    Vector<GraphicsLayer*> layerChildren;
    Vector<GraphicsLayer*>& childList = hasCompositedLayerMapping ? layerChildren : childLayersOfEnclosingLayer;

#if !ASSERT_DISABLED
    LayerListMutationDetector mutationChecker(layer->stackingNode());
#endif

    if (layer->stackingNode()->isStackingContainer()) {
        RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), NegativeZOrderChildren);
        while (RenderLayerStackingNode* curNode = iterator.next())
            rebuildCompositingLayerTree(curNode->layer(), childList, depth + 1);

        if (hasCompositedLayerMapping && currentCompositedLayerMapping->foregroundLayer())
            childList.append(currentCompositedLayerMapping->foregroundLayer());
    }

    RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), NormalFlowChildren | PositiveZOrderChildren);
    while (RenderLayerStackingNode* curNode = iterator.next())
        rebuildCompositingLayerTree(curNode->layer(), childList, depth + 1);

    if (hasCompositedLayerMapping) {
        bool parented = false;
        if (layer->renderer()->isRenderPart())
            parented = parentFrameContentLayers(toRenderPart(layer->renderer()));

        if (!parented)
            currentCompositedLayerMapping->parentForSublayers()->setChildren(layerChildren);

        if (!currentCompositedLayerMapping->hasClippingLayer() && !currentCompositedLayerMapping->hasScrollingLayer()) {
            if (GraphicsLayer* overflowControlLayer = currentCompositedLayerMapping->layerForHorizontalScrollbar()) {
                overflowControlLayer->removeFromParent();
                currentCompositedLayerMapping->parentForSublayers()->addChild(overflowControlLayer);
            }

            if (GraphicsLayer* overflowControlLayer = currentCompositedLayerMapping->layerForVerticalScrollbar()) {
                overflowControlLayer->removeFromParent();
                currentCompositedLayerMapping->parentForSublayers()->addChild(overflowControlLayer);
            }

            if (GraphicsLayer* overflowControlLayer = currentCompositedLayerMapping->layerForScrollCorner()) {
                overflowControlLayer->removeFromParent();
                currentCompositedLayerMapping->parentForSublayers()->addChild(overflowControlLayer);
            }
        }

        childLayersOfEnclosingLayer.append(currentCompositedLayerMapping->childForSuperlayers());
    }

    if (!depth) {
        int percentageIncreaseInPixels = static_cast<int>(pixelsAddedByPromotingAllTransitions / pixelsWithoutPromotingAllTransitions * 100);
        blink::Platform::current()->histogramCustomCounts("Renderer.PixelIncreaseFromTransitions", percentageIncreaseInPixels, 0, 1000, 50);
    }
}
