void RenderLayerCompositor::SquashingState::updateSquashingStateForNewMapping(CompositedLayerMappingPtr newCompositedLayerMapping, bool hasNewCompositedLayerMapping, IntPoint newOffsetFromAbsoluteForSquashingCLM)
{
    if (hasMostRecentMapping)
        mostRecentMapping->finishAccumulatingSquashingLayers(nextSquashedLayerIndex);

    nextSquashedLayerIndex = 0;
    mostRecentMapping = newCompositedLayerMapping;
    hasMostRecentMapping = hasNewCompositedLayerMapping;
    offsetFromAbsoluteForSquashingCLM = newOffsetFromAbsoluteForSquashingCLM;
}
