LayerTilerChromium::LayerTilerChromium(LayerRendererChromium* layerRenderer, const IntSize& tileSize)
    : m_skipsDraw(false)
    , m_layerRenderer(layerRenderer)
{
    setTileSize(tileSize);
}
