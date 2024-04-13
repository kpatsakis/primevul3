void RenderLayerCompositor::enableCompositingMode(bool enable /* = true */)
{
    if (enable != m_compositing) {
        m_compositing = enable;

        if (m_compositing) {
            ensureRootLayer();
            notifyIFramesOfCompositingChange();
        } else
            destroyRootLayer();
    }
}
