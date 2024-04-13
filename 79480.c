void RenderLayerCompositor::assignLayersToBackingsInternal(RenderLayer* layer, SquashingState& squashingState, bool& layersChanged)
{
    if (allocateOrClearCompositedLayerMapping(layer))
        layersChanged = true;

    if (layer->reflectionInfo())
        assignLayersToBackingsForReflectionLayer(layer->reflectionInfo()->reflectionLayer(), layersChanged);

    if (layerSquashingEnabled()) {

        if (requiresSquashing(layer->compositingReasons())) {
            ASSERT(!layer->hasCompositedLayerMapping());
            ASSERT(squashingState.hasMostRecentMapping);

            IntPoint offsetFromAbsoluteForSquashedLayer = computeOffsetFromAbsolute(layer);

            IntSize offsetFromSquashingCLM(offsetFromAbsoluteForSquashedLayer.x() - squashingState.offsetFromAbsoluteForSquashingCLM.x(),
                offsetFromAbsoluteForSquashedLayer.y() - squashingState.offsetFromAbsoluteForSquashingCLM.y());

            squashingState.mostRecentMapping->addRenderLayerToSquashingGraphicsLayer(layer, offsetFromSquashingCLM, squashingState.nextSquashedLayerIndex);
            squashingState.nextSquashedLayerIndex++;

            layersChanged = true;

            layer->clipper().clearClipRectsIncludingDescendants();
        } else {
            if (layer->compositingState() == PaintsIntoGroupedBacking) {
                ASSERT(layer->groupedMapping());
                layer->setGroupedMapping(0);
                layer->setShouldInvalidateNextBacking(true);
            }
        }
    }

    if (layer->shouldInvalidateNextBacking()) {
        if (layer->compositingState() == PaintsIntoGroupedBacking && squashingState.mostRecentMapping->squashingLayer()) {
            squashingState.mostRecentMapping->squashingLayer()->setNeedsDisplay();
        } else {
            RenderLayer* enclosingLayer = layer->enclosingCompositingLayerForRepaint(ExcludeSelf);
            if (enclosingLayer->hasCompositedLayerMapping())
                enclosingLayer->compositedLayerMapping()->setContentsNeedDisplay();
        }

        layer->setShouldInvalidateNextBacking(false);
    }

    if (layer->stackingNode()->isStackingContainer()) {
        RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), NegativeZOrderChildren);
        while (RenderLayerStackingNode* curNode = iterator.next())
            assignLayersToBackingsInternal(curNode->layer(), squashingState, layersChanged);
    }

    if (layerSquashingEnabled()) {
        if (layer->compositingState() == PaintsIntoOwnBacking || layer->compositingState() == HasOwnBackingButPaintsIntoAncestor) {
            ASSERT(!requiresSquashing(layer->compositingReasons()));
            IntPoint offsetFromAbsoluteForSquashingCLM = computeOffsetFromAbsolute(layer);
            squashingState.updateSquashingStateForNewMapping(layer->compositedLayerMapping(), layer->hasCompositedLayerMapping(), offsetFromAbsoluteForSquashingCLM);
        }
    }

    RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), NormalFlowChildren | PositiveZOrderChildren);
    while (RenderLayerStackingNode* curNode = iterator.next())
        assignLayersToBackingsInternal(curNode->layer(), squashingState, layersChanged);
}
