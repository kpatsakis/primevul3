    void createCompositingWebView()
    {
        m_fakeCompositingWebViewClient = adoptPtr(new FakeCompositingWebViewClient());
        m_webView = WebView::create(m_fakeCompositingWebViewClient.get());
        m_webView->settings()->setJavaScriptEnabled(true);
        m_webView->settings()->setForceCompositingMode(true);
        m_webView->settings()->setAcceleratedCompositingEnabled(true);
        m_webView->settings()->setAcceleratedCompositingForFixedPositionEnabled(true);
        m_webView->settings()->setAcceleratedCompositingForOverflowScrollEnabled(true);
        m_webView->settings()->setAcceleratedCompositingForScrollableFramesEnabled(true);
        m_webView->settings()->setCompositedScrollingForFramesEnabled(true);
        m_webView->settings()->setFixedPositionCreatesStackingContext(true);
        m_webView->initializeMainFrame(&m_fakeCompositingWebViewClient->m_fakeWebFrameClient);
    }
