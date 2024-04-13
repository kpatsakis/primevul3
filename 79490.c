bool RenderLayerCompositor::clipsCompositingDescendants(const RenderLayer* layer) const
{
    return layer->hasCompositingDescendant() && layer->renderer()->hasClipOrOverflowClip();
}
