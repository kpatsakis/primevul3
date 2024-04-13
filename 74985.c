IntRect LayerTilerChromium::tileLayerRect(int i, int j) const
{
    IntPoint anchor(i * m_tileSize.width(), j * m_tileSize.height());
    IntRect tile(anchor, m_tileSize);
    return tile;
}
