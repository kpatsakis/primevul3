void RenderLayerCompositor::recursiveRepaintLayer(RenderLayer* layer, const IntRect* rect)
{
    if (layer->compositingState() == PaintsIntoOwnBacking) {
        if (rect)
            layer->repainter().setBackingNeedsRepaintInRect(*rect);
        else
            layer->repainter().setBackingNeedsRepaint();
    }

#if !ASSERT_DISABLED
    LayerListMutationDetector mutationChecker(layer->stackingNode());
#endif

    unsigned childrenToVisit = NormalFlowChildren;
    if (layer->hasCompositingDescendant())
        childrenToVisit |= PositiveZOrderChildren | NegativeZOrderChildren;
    RenderLayerStackingNodeIterator iterator(*layer->stackingNode(), childrenToVisit);
    while (RenderLayerStackingNode* curNode = iterator.next()) {
        if (rect) {
            IntRect childRect(*rect);
            curNode->layer()->convertToPixelSnappedLayerCoords(layer, childRect);
            recursiveRepaintLayer(curNode->layer(), &childRect);
        } else {
            recursiveRepaintLayer(curNode->layer());
        }
    }
}
