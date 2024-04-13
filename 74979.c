void LayerTilerChromium::growLayerToContain(const IntRect& contentRect)
{
    IntRect layerRect = contentRectToLayerRect(contentRect);
    IntSize layerSize = IntSize(layerRect.maxX(), layerRect.maxY());

    IntSize newSize = layerSize.expandedTo(m_layerSize);
    resizeLayer(newSize);
}
