void simulateMultiTargetZoom(WebViewImpl* webViewImpl, const WebRect& rect, float& scale)
{
    if (webViewImpl->zoomToMultipleTargetsRect(rect))
        simulatePageScale(webViewImpl, scale);
}
