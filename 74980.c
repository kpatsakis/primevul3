void LayerTilerChromium::invalidateEntireLayer()
{
    for (size_t i = 0; i < m_tiles.size(); ++i) {
        if (m_tiles[i])
            m_unusedTiles.append(m_tiles[i].release());
    }
    m_tiles.clear();

    m_layerSize = IntSize();
    m_layerTileSize = IntSize();
    m_lastUpdateLayerRect = IntRect();
}
