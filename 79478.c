void RenderLayerCompositor::assignLayersToBackings(RenderLayer* updateRoot, bool& layersChanged)
{
    SquashingState squashingState;
    assignLayersToBackingsInternal(updateRoot, squashingState, layersChanged);
    if (squashingState.hasMostRecentMapping)
        squashingState.mostRecentMapping->finishAccumulatingSquashingLayers(squashingState.nextSquashedLayerIndex);
}
