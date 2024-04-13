RenderLayer* RenderLayerCompositor::enclosingNonStackingClippingLayer(const RenderLayer* layer) const
{
    for (RenderLayer* curr = layer->parent(); curr != 0; curr = curr->parent()) {
        if (curr->stackingNode()->isStackingContainer())
            return 0;

        if (curr->renderer()->hasClipOrOverflowClip())
            return curr;
    }
    return 0;
}
