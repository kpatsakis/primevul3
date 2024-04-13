void FrameLoader::loadItem(HistoryItem* item, FrameLoadType loadType)
{
    if (!m_frame->page())
        return;

    KURL itemURL = item->url();
    KURL itemOriginalURL = item->originalURL();
    KURL currentURL;
    if (documentLoader())
        currentURL = documentLoader()->url();
    RefPtr<FormData> formData = item->formData();

    bool shouldScroll = !formData && !(history()->currentItem() && history()->currentItem()->formData()) && history()->urlsMatchItem(item);

#if ENABLE(WML)
    if (frameContainsWMLContent(m_frame))
        shouldScroll = false;
#endif

    if (shouldScroll) {
        history()->saveScrollPositionAndViewStateToItem(history()->currentItem());

        if (FrameView* view = m_frame->view())
            view->setWasScrolledByUser(false);

        history()->setCurrentItem(item);


        scrollToAnchor(item->url());
    
        history()->restoreScrollPositionAndViewState();
        
        documentLoader()->replaceRequestURLForAnchorScroll(itemURL);

        m_client->dispatchDidChangeLocationWithinPage();
        
        m_client->didFinishLoad();
    } else {
        history()->setProvisionalItem(item);

        bool inPageCache = false;
        
        if (RefPtr<CachedPage> cachedPage = pageCache()->get(item)) {
            double interval = currentTime() - cachedPage->timeStamp();
            
            if (interval <= 1800) {
                loadWithDocumentLoader(cachedPage->documentLoader(), loadType, 0);   
                inPageCache = true;
            } else {
                LOG(PageCache, "Not restoring page for %s from back/forward cache because cache entry has expired", history()->provisionalItem()->url().string().ascii().data());
                pageCache()->remove(item);
            }
        }
        
        if (!inPageCache) {
            bool addedExtraFields = false;
            ResourceRequest request(itemURL);

            if (!item->referrer().isNull())
                request.setHTTPReferrer(item->referrer());
            
            NavigationAction action;
            if (formData) {
                
                formData->generateFiles(m_frame->page()->chrome()->client());

                request.setHTTPMethod("POST");
                request.setHTTPBody(formData);
                request.setHTTPContentType(item->formContentType());
                RefPtr<SecurityOrigin> securityOrigin = SecurityOrigin::createFromString(item->referrer());
                addHTTPOriginIfNeeded(request, securityOrigin->toString());
        
                addExtraFieldsToRequest(request, m_loadType, true, formData);
                addedExtraFields = true;
                
                
                if (ResourceHandle::willLoadFromCache(request, m_frame))
                    action = NavigationAction(itemURL, loadType, false);
                else {
                    request.setCachePolicy(ReloadIgnoringCacheData);
                    action = NavigationAction(itemURL, NavigationTypeFormResubmitted);
                }
            } else {
                switch (loadType) {
                    case FrameLoadTypeReload:
                    case FrameLoadTypeReloadFromOrigin:
                        request.setCachePolicy(ReloadIgnoringCacheData);
                        break;
                    case FrameLoadTypeBack:
                    case FrameLoadTypeBackWMLDeckNotAccessible:
                    case FrameLoadTypeForward:
                    case FrameLoadTypeIndexedBackForward:
                        if (itemURL.protocol() != "https")
                            request.setCachePolicy(ReturnCacheDataElseLoad);
                        break;
                    case FrameLoadTypeStandard:
                    case FrameLoadTypeRedirectWithLockedBackForwardList:
                        break;
                    case FrameLoadTypeSame:
                    default:
                        ASSERT_NOT_REACHED();
                }

                action = NavigationAction(itemOriginalURL, loadType, false);
            }
            
            if (!addedExtraFields)
                addExtraFieldsToRequest(request, m_loadType, true, formData);

            loadWithNavigationAction(request, action, false, loadType, 0);
        }
    }
}
