PassOwnPtr<LayerTilerChromium> LayerTilerChromium::create(LayerRendererChromium* layerRenderer, const IntSize& tileSize)
{
    if (!layerRenderer || tileSize.isEmpty())
        return 0;

    return adoptPtr(new LayerTilerChromium(layerRenderer, tileSize));
}
