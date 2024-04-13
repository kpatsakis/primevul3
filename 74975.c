void LayerTilerChromium::contentRectToTileIndices(const IntRect& contentRect, int &left, int &top, int &right, int &bottom) const
{
    const IntRect layerRect = contentRectToLayerRect(contentRect);

    left = layerRect.x() / m_tileSize.width();
    top = layerRect.y() / m_tileSize.height();
    right = (layerRect.maxX() - 1) / m_tileSize.width();
    bottom = (layerRect.maxY() - 1) / m_tileSize.height();
}
