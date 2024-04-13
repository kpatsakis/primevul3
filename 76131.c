static WebCore::ViewportAttributes _ewk_view_viewport_attributes_compute(const Ewk_View_Private_Data* priv)
{
    int desktopWidth = 980;
    int deviceDPI = ewk_util_dpi_get();

    WebCore::IntRect availableRect = enclosingIntRect(priv->page->chrome()->client()->pageRect());
    WebCore::IntRect deviceRect = enclosingIntRect(priv->page->chrome()->client()->windowRect());

    WebCore::ViewportAttributes attributes = WebCore::computeViewportAttributes(priv->viewportArguments, desktopWidth, deviceRect.width(), deviceRect.height(), deviceDPI, availableRect.size());
    WebCore::restrictMinimumScaleFactorToViewportSize(attributes, availableRect.size());
    WebCore::restrictScaleFactorToInitialScaleIfNotUserScalable(attributes);

    return attributes;
}
