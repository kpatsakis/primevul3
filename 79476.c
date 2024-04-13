void RenderLayerCompositor::addToOverlapMapRecursive(OverlapMap& overlapMap, RenderLayer* layer, RenderLayer* ancestorLayer)
{
    if (!canBeComposited(layer) || overlapMap.contains(layer))
        return;

    if (ancestorLayer)
        overlapMap.geometryMap().pushMappingsToAncestor(layer, ancestorLayer);

    IntRect bounds;
    bool haveComputedBounds = false;
    addToOverlapMap(overlapMap, layer, bounds, haveComputedBounds);

#if !ASSERT_DISABLED
    LayerListMutationDetector mutationChecker(layer->stackingNode());
#endif

    RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), AllChildren);
    while (RenderLayerStackingNode* curNode = iterator.next())
        addToOverlapMapRecursive(overlapMap, curNode->layer(), layer);

    if (ancestorLayer)
        overlapMap.geometryMap().popMappingsToAncestor(ancestorLayer);
}
