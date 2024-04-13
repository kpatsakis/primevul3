static int adjustForLocalZoom(LayoutUnit value, RenderObject* renderer)
{
    float zoomFactor = localZoomForRenderer(renderer);
    if (zoomFactor == 1)
        return value;
    return lroundf(value / zoomFactor);
}
