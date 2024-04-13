bool RenderLayerCompositor::updateLayerIfViewportConstrained(RenderLayer* layer)
{
    RenderLayer::ViewportConstrainedNotCompositedReason viewportConstrainedNotCompositedReason = RenderLayer::NoNotCompositedReason;
    requiresCompositingForPosition(layer->renderer(), layer, &viewportConstrainedNotCompositedReason);

    if (layer->viewportConstrainedNotCompositedReason() != viewportConstrainedNotCompositedReason) {
        ASSERT(layer->renderer()->style()->position() == FixedPosition);

        layer->setViewportConstrainedNotCompositedReason(viewportConstrainedNotCompositedReason);
        return true;
    }
    return false;
}
