bool RenderLayerCompositor::requiresCompositingForBackfaceVisibilityHidden(RenderObject* renderer) const
{
    return canRender3DTransforms() && renderer->style()->backfaceVisibility() == BackfaceVisibilityHidden;
}
