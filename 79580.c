void RenderLayerCompositor::updateOverflowControlsLayers()
{
#if USE(RUBBER_BANDING)
    if (requiresOverhangLayers()) {
        if (!m_layerForOverhangShadow) {
            m_layerForOverhangShadow = GraphicsLayer::create(graphicsLayerFactory(), this);
            OverscrollTheme::theme()->setUpOverhangShadowLayer(m_layerForOverhangShadow.get());
            OverscrollTheme::theme()->updateOverhangShadowLayer(m_layerForOverhangShadow.get(), m_rootContentLayer.get());
            m_scrollLayer->addChild(m_layerForOverhangShadow.get());
        }
    } else {
        if (m_layerForOverhangShadow) {
            m_layerForOverhangShadow->removeFromParent();
            m_layerForOverhangShadow = nullptr;
        }
    }
#endif

    if (requiresHorizontalScrollbarLayer()) {
        if (!m_layerForHorizontalScrollbar) {
            m_layerForHorizontalScrollbar = GraphicsLayer::create(graphicsLayerFactory(), this);
            m_overflowControlsHostLayer->addChild(m_layerForHorizontalScrollbar.get());

            if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
                scrollingCoordinator->scrollableAreaScrollbarLayerDidChange(m_renderView->frameView(), HorizontalScrollbar);
        }
    } else if (m_layerForHorizontalScrollbar) {
        m_layerForHorizontalScrollbar->removeFromParent();
        m_layerForHorizontalScrollbar = nullptr;

        if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
            scrollingCoordinator->scrollableAreaScrollbarLayerDidChange(m_renderView->frameView(), HorizontalScrollbar);
    }

    if (requiresVerticalScrollbarLayer()) {
        if (!m_layerForVerticalScrollbar) {
            m_layerForVerticalScrollbar = GraphicsLayer::create(graphicsLayerFactory(), this);
            m_overflowControlsHostLayer->addChild(m_layerForVerticalScrollbar.get());

            if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
                scrollingCoordinator->scrollableAreaScrollbarLayerDidChange(m_renderView->frameView(), VerticalScrollbar);
        }
    } else if (m_layerForVerticalScrollbar) {
        m_layerForVerticalScrollbar->removeFromParent();
        m_layerForVerticalScrollbar = nullptr;

        if (ScrollingCoordinator* scrollingCoordinator = this->scrollingCoordinator())
            scrollingCoordinator->scrollableAreaScrollbarLayerDidChange(m_renderView->frameView(), VerticalScrollbar);
    }

    if (requiresScrollCornerLayer()) {
        if (!m_layerForScrollCorner) {
            m_layerForScrollCorner = GraphicsLayer::create(graphicsLayerFactory(), this);
            m_overflowControlsHostLayer->addChild(m_layerForScrollCorner.get());
        }
    } else if (m_layerForScrollCorner) {
        m_layerForScrollCorner->removeFromParent();
        m_layerForScrollCorner = nullptr;
    }

    m_renderView->frameView()->positionScrollbarLayers();
}
