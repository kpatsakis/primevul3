void RenderLayerCompositor::addToOverlapMap(OverlapMap& overlapMap, RenderLayer* layer, IntRect& layerBounds, bool& boundsComputed)
{
    if (layer->isRootLayer())
        return;

    if (!boundsComputed) {
        layerBounds = enclosingIntRect(overlapMap.geometryMap().absoluteRect(layer->overlapBounds()));
        if (layerBounds.isEmpty())
            layerBounds.setSize(IntSize(1, 1));
        boundsComputed = true;
    }

    IntRect clipRect = pixelSnappedIntRect(layer->clipper().backgroundClipRect(ClipRectsContext(rootRenderLayer(), 0, AbsoluteClipRects)).rect()); // FIXME: Incorrect for CSS regions.
    clipRect.intersect(layerBounds);
    overlapMap.add(layer, clipRect);
}
