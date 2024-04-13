bool RenderLayerCompositor::canRender3DTransforms() const
{
    return hasAcceleratedCompositing() && (m_compositingTriggers & ChromeClient::ThreeDTransformTrigger);
}
