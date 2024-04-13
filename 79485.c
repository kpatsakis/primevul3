bool RenderLayerCompositor::canAccelerateVideoRendering(RenderVideo* o) const
{
    if (!m_hasAcceleratedCompositing)
        return false;

    return o->supportsAcceleratedRendering();
}
