GraphicsContext3D* LayerTilerChromium::layerRendererContext() const
{
    ASSERT(layerRenderer());
    return layerRenderer()->context();
}
