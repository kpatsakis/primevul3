LayoutRect RenderLayerCompositor::calculateCompositedBounds(const RenderLayer* layer, const RenderLayer* ancestorLayer) const
{
    if (!canBeComposited(layer))
        return LayoutRect();

    RenderLayer::CalculateLayerBoundsFlags flags = RenderLayer::DefaultCalculateLayerBoundsFlags | RenderLayer::ExcludeHiddenDescendants | RenderLayer::DontConstrainForMask;
#if HAVE(COMPOSITOR_FILTER_OUTSETS)
    if (!layer->paintsWithFilters())
        flags &= ~RenderLayer::IncludeLayerFilterOutsets;
#endif
    return layer->calculateLayerBounds(ancestorLayer, 0, flags);
}
