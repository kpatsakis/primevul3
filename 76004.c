IntRect RenderBox::maskClipRect()
{
    IntRect bbox = borderBoxRect();
    if (style()->maskBoxImage().image())
        return bbox;
    
    IntRect result;
    for (const FillLayer* maskLayer = style()->maskLayers(); maskLayer; maskLayer = maskLayer->next()) {
        if (maskLayer->image()) {
            BackgroundImageGeometry geometry;
            calculateBackgroundImageGeometry(maskLayer, bbox, geometry);
            result.unite(geometry.destRect());
        }
    }
    return result;
}
