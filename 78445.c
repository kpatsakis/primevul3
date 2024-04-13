    virtual WebNavigationPolicy decidePolicyForNavigation(
        WebFrame*, const WebURLRequest&, WebNavigationType,
        WebNavigationPolicy defaultPolicy, bool isRedirect)
    {
        EXPECT_FALSE(isRedirect);
        return WebNavigationPolicyCurrentTab;
    }
