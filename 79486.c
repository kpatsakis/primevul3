bool RenderLayerCompositor::canBeComposited(const RenderLayer* layer) const
{
    return m_hasAcceleratedCompositing && layer->isSelfPaintingLayer() && layer->renderer()->flowThreadState() == RenderObject::NotInsideFlowThread;
}
