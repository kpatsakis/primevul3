bool FrameLoader::loadProvisionalItemFromCachedPage()
{
    RefPtr<CachedPage> cachedPage = pageCache()->get(history()->provisionalItem());
    if (!cachedPage || !cachedPage->document())
        return false;

    DocumentLoader *provisionalLoader = provisionalDocumentLoader();
    LOG(PageCache, "WebCorePageCache: FrameLoader %p loading provisional DocumentLoader %p with URL '%s' from CachedPage %p", this, provisionalLoader, provisionalLoader->url().string().utf8().data(), cachedPage.get());
    
    provisionalLoader->prepareForLoadStart();

    m_loadingFromCachedPage = true;

    provisionalLoader->setCommitted(true);
    commitProvisionalLoad(cachedPage);
    
    return true;
}
