String RenderLayerCompositor::debugName(const GraphicsLayer* graphicsLayer)
{
    String name;
    if (graphicsLayer == m_rootContentLayer.get()) {
        name = "Content Root Layer";
#if USE(RUBBER_BANDING)
    } else if (graphicsLayer == m_layerForOverhangShadow.get()) {
        name = "Overhang Areas Shadow";
#endif
    } else if (graphicsLayer == m_overflowControlsHostLayer.get()) {
        name = "Overflow Controls Host Layer";
    } else if (graphicsLayer == m_layerForHorizontalScrollbar.get()) {
        name = "Horizontal Scrollbar Layer";
    } else if (graphicsLayer == m_layerForVerticalScrollbar.get()) {
        name = "Vertical Scrollbar Layer";
    } else if (graphicsLayer == m_layerForScrollCorner.get()) {
        name = "Scroll Corner Layer";
    } else if (graphicsLayer == m_containerLayer.get()) {
        name = "Frame Clipping Layer";
    } else if (graphicsLayer == m_scrollLayer.get()) {
        name = "Frame Scrolling Layer";
    } else {
        ASSERT_NOT_REACHED();
    }

    return name;
}
