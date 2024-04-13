void FrameLoader::loadHistoryItem(HistoryItem* item)
{
    m_requestedHistoryItem = item;
    HistoryItem* currentItem = history()->currentItem();

    if (currentItem && item->shouldDoSameDocumentNavigationTo(currentItem)) {
        history()->setCurrentItem(item);
        loadInSameDocument(item->url(), item->stateObject(), false);
        return;
    }

    history()->setProvisionalItem(item);

    RefPtr<FormData> formData = item->formData();
    ResourceRequest request(item->url());
    request.setHTTPReferrer(item->referrer());
    if (formData) {
        request.setHTTPMethod("POST");
        request.setHTTPBody(formData);
        request.setHTTPContentType(item->formContentType());
        RefPtr<SecurityOrigin> securityOrigin = SecurityOrigin::createFromString(item->referrer());
        addHTTPOriginIfNeeded(request, securityOrigin->toString());
    }

    loadWithNavigationAction(request, NavigationAction(request, FrameLoadTypeBackForward, false), FrameLoadTypeBackForward, 0, SubstituteData());
}
