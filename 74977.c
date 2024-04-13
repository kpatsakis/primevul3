LayerTilerChromium::Tile* LayerTilerChromium::createTile(int i, int j)
{
    const int index = tileIndex(i, j);
    ASSERT(!m_tiles[index]);

    if (m_unusedTiles.size() > 0) {
        m_tiles[index] = m_unusedTiles.last().release();
        m_unusedTiles.removeLast();
    } else {
        GraphicsContext3D* context = layerRendererContext();
        TextureManager* manager = layerRenderer()->textureManager();
        OwnPtr<Tile> tile = adoptPtr(new Tile(LayerTexture::create(context, manager)));
        m_tiles[index] = tile.release();
    }

    m_tiles[index]->m_dirtyLayerRect = tileLayerRect(i, j);
    return m_tiles[index].get();
}
