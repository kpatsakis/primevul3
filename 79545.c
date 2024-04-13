bool RenderLayerCompositor::requiresCompositingForFrame(RenderObject* renderer) const
{
    if (!renderer->isRenderPart())
        return false;

    RenderPart* frameRenderer = toRenderPart(renderer);

    if (!frameRenderer->requiresAcceleratedCompositing())
        return false;

    if (frameRenderer->node() && frameRenderer->node()->isFrameOwnerElement() && toHTMLFrameOwnerElement(frameRenderer->node())->contentFrame() && toHTMLFrameOwnerElement(frameRenderer->node())->contentFrame()->remotePlatformLayer())
        return true;

    m_needsToRecomputeCompositingRequirements = true;

    RenderLayerCompositor* innerCompositor = frameContentsCompositor(frameRenderer);
    if (!innerCompositor)
        return false;

    if (renderer->needsLayout())
        return frameRenderer->hasLayer() && frameRenderer->layer()->hasCompositedLayerMapping();

    IntRect contentBox = pixelSnappedIntRect(frameRenderer->contentBoxRect());
    return contentBox.height() * contentBox.width() > 0;
}
