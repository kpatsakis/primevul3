static CachedResource* createResource(CachedResource::Type type, const KURL& url, const String& charset)
{
    switch (type) {
    case CachedResource::ImageResource:
        return new CachedImage(url.string());
    case CachedResource::CSSStyleSheet:
        return new CachedCSSStyleSheet(url.string(), charset);
    case CachedResource::Script:
        return new CachedScript(url.string(), charset);
    case CachedResource::FontResource:
        return new CachedFont(url.string());
#if ENABLE(XSLT)
    case CachedResource::XSLStyleSheet:
        return new CachedXSLStyleSheet(url.string());
#endif
#if ENABLE(XBL)
    case CachedResource::XBLStyleSheet:
        return new CachedXBLDocument(url.string());
#endif
    default:
        break;
    }

    return 0;
}
