void RenderLayerCompositor::updateLayerTreeGeometry(RenderLayer* layer)
{
    updateGraphicsLayersMappedToRenderLayer(layer);

#if !ASSERT_DISABLED
    LayerListMutationDetector mutationChecker(layer->stackingNode());
#endif

    RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), AllChildren);
    while (RenderLayerStackingNode* curNode = iterator.next())
        updateLayerTreeGeometry(curNode->layer());
}
