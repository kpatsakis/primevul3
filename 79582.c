void RenderLayerCompositor::updateRootLayerPosition()
{
    if (m_rootContentLayer) {
        const IntRect& documentRect = m_renderView->documentRect();
        m_rootContentLayer->setSize(documentRect.size());
        m_rootContentLayer->setPosition(documentRect.location());
#if USE(RUBBER_BANDING)
        if (m_layerForOverhangShadow)
            OverscrollTheme::theme()->updateOverhangShadowLayer(m_layerForOverhangShadow.get(), m_rootContentLayer.get());
#endif
    }
    if (m_containerLayer) {
        FrameView* frameView = m_renderView->frameView();
        m_containerLayer->setSize(frameView->unscaledVisibleContentSize());
    }
}
