void RenderLayerCompositor::layerWillBeRemoved(RenderLayer* parent, RenderLayer* child)
{
    if (!child->hasCompositedLayerMapping() || parent->renderer()->documentBeingDestroyed())
        return;

    removeViewportConstrainedLayer(child);

    {
        DisableCompositingQueryAsserts disabler;
        repaintInCompositedAncestor(child, child->compositedLayerMapping()->compositedBounds());
    }

    setCompositingParent(child, 0);
    setCompositingLayersNeedRebuild();
}
