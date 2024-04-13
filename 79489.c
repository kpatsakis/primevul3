bool RenderLayerCompositor::clippedByAncestor(const RenderLayer* layer) const
{
    if (!layer->hasCompositedLayerMapping() || !layer->parent())
        return false;

    const RenderLayer* compositingAncestor = layer->ancestorCompositingLayer();
    if (!compositingAncestor)
        return false;

    const RenderLayer* computeClipRoot = 0;
    const RenderLayer* curr = layer;
    while (curr) {
        const RenderLayer* next = curr->parent();
        if (next == compositingAncestor) {
            computeClipRoot = curr;
            break;
        }
        curr = next;
    }

    if (!computeClipRoot || computeClipRoot == layer)
        return false;

    return layer->clipper().backgroundClipRect(ClipRectsContext(computeClipRoot, 0, TemporaryClipRects)).rect() != PaintInfo::infiniteRect(); // FIXME: Incorrect for CSS regions.
}
