void FrameLoader::cachePageForHistoryItem(HistoryItem* item)
{
    if (!canCachePage() || item->isInPageCache())
        return;

    pageHidden();
    
    if (Page* page = m_frame->page()) {
        RefPtr<CachedPage> cachedPage = CachedPage::create(page);
        pageCache()->add(item, cachedPage.release());
    }
}
