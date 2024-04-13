FixedPositionViewportConstraints RenderLayerCompositor::computeFixedViewportConstraints(RenderLayer* layer) const
{
    ASSERT(layer->hasCompositedLayerMapping());

    FrameView* frameView = m_renderView->frameView();
    LayoutRect viewportRect = frameView->viewportConstrainedVisibleContentRect();

    FixedPositionViewportConstraints constraints;

    GraphicsLayer* graphicsLayer = layer->compositedLayerMapping()->mainGraphicsLayer();

    constraints.setLayerPositionAtLastLayout(graphicsLayer->position());
    constraints.setViewportRectAtLastLayout(viewportRect);

    RenderStyle* style = layer->renderer()->style();
    if (!style->left().isAuto())
        constraints.addAnchorEdge(ViewportConstraints::AnchorEdgeLeft);

    if (!style->right().isAuto())
        constraints.addAnchorEdge(ViewportConstraints::AnchorEdgeRight);

    if (!style->top().isAuto())
        constraints.addAnchorEdge(ViewportConstraints::AnchorEdgeTop);

    if (!style->bottom().isAuto())
        constraints.addAnchorEdge(ViewportConstraints::AnchorEdgeBottom);

    if (style->left().isAuto() && style->right().isAuto())
        constraints.addAnchorEdge(ViewportConstraints::AnchorEdgeLeft);

    if (style->top().isAuto() && style->bottom().isAuto())
        constraints.addAnchorEdge(ViewportConstraints::AnchorEdgeTop);

    return constraints;
}
