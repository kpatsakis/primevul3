StickyPositionViewportConstraints RenderLayerCompositor::computeStickyViewportConstraints(RenderLayer* layer) const
{
    ASSERT(layer->hasCompositedLayerMapping());
    ASSERT(!layer->enclosingOverflowClipLayer(ExcludeSelf));

    FrameView* frameView = m_renderView->frameView();
    LayoutRect viewportRect = frameView->viewportConstrainedVisibleContentRect();

    StickyPositionViewportConstraints constraints;

    RenderBoxModelObject* renderer = toRenderBoxModelObject(layer->renderer());

    renderer->computeStickyPositionConstraints(constraints, viewportRect);

    GraphicsLayer* graphicsLayer = layer->compositedLayerMapping()->mainGraphicsLayer();

    constraints.setLayerPositionAtLastLayout(graphicsLayer->position());
    constraints.setStickyOffsetAtLastLayout(renderer->stickyPositionOffset());

    return constraints;
}
