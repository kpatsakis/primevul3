void RenderLayerCompositor::destroyRootLayer()
{
    if (!m_rootContentLayer)
        return;

    detachRootLayer();

#if USE(RUBBER_BANDING)
    if (m_layerForOverhangShadow) {
        m_layerForOverhangShadow->removeFromParent();
        m_layerForOverhangShadow = nullptr;
    }
#endif

    if (m_layerForHorizontalScrollbar) {
        m_layerForHorizontalScrollbar->removeFromParent();
        m_layerForHorizontalScrollbar = nullptr;
        if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
            scrollingCoordinator->scrollableAreaScrollbarLayerDidChange(m_renderView->frameView(), HorizontalScrollbar);
        if (Scrollbar* horizontalScrollbar = m_renderView->frameView()->verticalScrollbar())
            m_renderView->frameView()->invalidateScrollbar(horizontalScrollbar, IntRect(IntPoint(0, 0), horizontalScrollbar->frameRect().size()));
    }

    if (m_layerForVerticalScrollbar) {
        m_layerForVerticalScrollbar->removeFromParent();
        m_layerForVerticalScrollbar = nullptr;
        if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
            scrollingCoordinator->scrollableAreaScrollbarLayerDidChange(m_renderView->frameView(), VerticalScrollbar);
        if (Scrollbar* verticalScrollbar = m_renderView->frameView()->verticalScrollbar())
            m_renderView->frameView()->invalidateScrollbar(verticalScrollbar, IntRect(IntPoint(0, 0), verticalScrollbar->frameRect().size()));
    }

    if (m_layerForScrollCorner) {
        m_layerForScrollCorner = nullptr;
        m_renderView->frameView()->invalidateScrollCorner(m_renderView->frameView()->scrollCornerRect());
    }

    if (m_overflowControlsHostLayer) {
        m_overflowControlsHostLayer = nullptr;
        m_containerLayer = nullptr;
        m_scrollLayer = nullptr;
    }
    ASSERT(!m_scrollLayer);
    m_rootContentLayer = nullptr;
}
