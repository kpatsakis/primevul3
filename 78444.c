static WebView* createWebViewForTextSelection(const std::string& url)
{
    WebView* webView = FrameTestHelpers::createWebViewAndLoad(url, true);
    webView->settings()->setDefaultFontSize(12);
    webView->enableFixedLayoutMode(false);
    webView->resize(WebSize(640, 480));
    return webView;
}
