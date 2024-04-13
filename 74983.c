void LayerTilerChromium::setTileSize(const IntSize& size)
{
    if (m_tileSize == size)
        return;

    reset();

    m_tileSize = size;
    m_tilePixels = adoptArrayPtr(new uint8_t[m_tileSize.width() * m_tileSize.height() * 4]);
}
