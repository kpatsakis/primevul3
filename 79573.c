void RenderLayerCompositor::updateCompositingDescendantGeometry(RenderLayerStackingNode* compositingAncestor, RenderLayer* layer, bool compositedChildrenOnly)
{
    if (layer->stackingNode() != compositingAncestor) {
        if (layer->hasCompositedLayerMapping()) {
            CompositedLayerMappingPtr compositedLayerMapping = layer->compositedLayerMapping();
            compositedLayerMapping->updateCompositedBounds();

            if (layer->reflectionInfo()) {
                RenderLayer* reflectionLayer = layer->reflectionInfo()->reflectionLayer();
                if (reflectionLayer->hasCompositedLayerMapping())
                    reflectionLayer->compositedLayerMapping()->updateCompositedBounds();
            }

            compositedLayerMapping->updateGraphicsLayerGeometry();
            if (compositedChildrenOnly)
                return;
        }
    }

    if (layer->reflectionInfo())
        updateCompositingDescendantGeometry(compositingAncestor, layer->reflectionInfo()->reflectionLayer(), compositedChildrenOnly);

    if (!layer->hasCompositingDescendant())
        return;

#if !ASSERT_DISABLED
    LayerListMutationDetector mutationChecker(layer->stackingNode());
#endif

    RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), AllChildren);
    while (RenderLayerStackingNode* curNode = iterator.next())
        updateCompositingDescendantGeometry(compositingAncestor, curNode->layer(), compositedChildrenOnly);
}
