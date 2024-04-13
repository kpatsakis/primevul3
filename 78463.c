void setScaleAndScrollAndLayout(WebKit::WebView* webView, WebPoint scroll, float scale)
{
    webView->setPageScaleFactor(scale, WebPoint(scroll.x, scroll.y));
    webView->layout();
}
