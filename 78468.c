    void testResizeYieldsCorrectScrollAndScale(const char* url,
                                               const float initialPageScaleFactor,
                                               const WebSize scrollOffset,
                                               const WebSize viewportSize,
                                               const bool shouldScaleRelativeToViewportWidth) {
        registerMockedHttpURLLoad(url);

        const float aspectRatio = static_cast<float>(viewportSize.width) / viewportSize.height;

        m_webView = FrameTestHelpers::createWebViewAndLoad(m_baseURL + url, true);
        m_webView->settings()->setViewportEnabled(true);
        m_webView->enableFixedLayoutMode(true);
        WebViewImpl* webViewImpl = static_cast<WebViewImpl*>(m_webView);

        {
            webViewImpl->resize(WebSize(viewportSize.width, viewportSize.height));
            m_webView->setPageScaleFactor(initialPageScaleFactor, WebPoint());
            webViewImpl->resize(WebSize(viewportSize.height, viewportSize.width));
            float expectedPageScaleFactor = initialPageScaleFactor * (shouldScaleRelativeToViewportWidth ? 1 / aspectRatio : 1);
            EXPECT_NEAR(expectedPageScaleFactor, webViewImpl->pageScaleFactor(), 0.05f);
            EXPECT_EQ(WebSize(), webViewImpl->mainFrame()->scrollOffset());
        }

        {
            webViewImpl->resize(WebSize(viewportSize.width, viewportSize.height));
            m_webView->setPageScaleFactor(initialPageScaleFactor, WebPoint());
            webViewImpl->mainFrame()->setScrollOffset(WebSize(0, 10));
            webViewImpl->resize(WebSize(viewportSize.width, viewportSize.height * 1.25f));
            EXPECT_EQ(initialPageScaleFactor, webViewImpl->pageScaleFactor());
            webViewImpl->resize(WebSize(viewportSize.width, viewportSize.height * .8f));
            EXPECT_EQ(initialPageScaleFactor, webViewImpl->pageScaleFactor());
        }

        {
            webViewImpl->resize(WebSize(viewportSize.height, viewportSize.width));
            float pageScaleFactor = webViewImpl->pageScaleFactor();
            webViewImpl->resize(WebSize(viewportSize.width, viewportSize.height));
            float expectedPageScaleFactor = pageScaleFactor * (shouldScaleRelativeToViewportWidth ? aspectRatio : 1);
            EXPECT_NEAR(expectedPageScaleFactor, webViewImpl->pageScaleFactor(), 0.05f);
            webViewImpl->mainFrame()->setScrollOffset(scrollOffset);

            WebCore::IntPoint anchorPoint = WebCore::IntPoint(scrollOffset) + WebCore::IntPoint(viewportSize.width / 2, 0);
            RefPtr<WebCore::Node> anchorNode = webViewImpl->mainFrameImpl()->frame()->eventHandler()->hitTestResultAtPoint(anchorPoint, HitTestRequest::ReadOnly | HitTestRequest::Active | HitTestRequest::DisallowShadowContent).innerNode();
            ASSERT(anchorNode);

            pageScaleFactor = webViewImpl->pageScaleFactor();
            const WebCore::FloatSize preResizeRelativeOffset
                = computeRelativeOffset(anchorPoint, anchorNode->boundingBox());
            webViewImpl->resize(WebSize(viewportSize.height, viewportSize.width));
            WebCore::IntPoint newAnchorPoint = WebCore::IntPoint(webViewImpl->mainFrame()->scrollOffset()) + WebCore::IntPoint(viewportSize.height / 2, 0);
            const WebCore::FloatSize postResizeRelativeOffset
                = computeRelativeOffset(newAnchorPoint, anchorNode->boundingBox());
            EXPECT_NEAR(preResizeRelativeOffset.width(), postResizeRelativeOffset.width(), 0.15f);
            expectedPageScaleFactor = pageScaleFactor * (shouldScaleRelativeToViewportWidth ? 1 / aspectRatio : 1);
            EXPECT_NEAR(expectedPageScaleFactor, webViewImpl->pageScaleFactor(), 0.05f);
        }
    }
