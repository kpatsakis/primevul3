int LayerTilerChromium::tileIndex(int i, int j) const
{
    ASSERT(i >= 0 && j >= 0 && i < m_layerTileSize.width() && j < m_layerTileSize.height());
    return i + j * m_layerTileSize.width();
}
