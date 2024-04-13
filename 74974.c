IntRect LayerTilerChromium::contentRectToLayerRect(const IntRect& contentRect) const
{
    IntPoint pos(contentRect.x() - m_layerPosition.x(), contentRect.y() - m_layerPosition.y());
    IntRect layerRect(pos, contentRect.size());

    if (pos.x() < 0 || pos.y() < 0)
        layerRect = IntRect(IntPoint(0, 0), IntSize(contentRect.width() + pos.x(), contentRect.height() + pos.y()));
    return layerRect;
}
