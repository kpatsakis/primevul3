void FrameLoader::loadInSameDocument(const KURL& url, PassRefPtr<SerializedScriptValue> stateObject, bool isNewNavigation)
{
    ASSERT(!stateObject || (stateObject && !isNewNavigation));

    KURL oldURL = m_frame->document()->url();
    bool hashChange = equalIgnoringFragmentIdentifier(url, oldURL) && url.fragmentIdentifier() != oldURL.fragmentIdentifier();
    if (hashChange) {
        m_frame->eventHandler()->stopAutoscrollTimer();
        m_frame->document()->enqueueHashchangeEvent(oldURL, url);
    }
    m_documentLoader->setIsClientRedirect((m_startingClientRedirect && !isNewNavigation) || !UserGestureIndicator::processingUserGesture());
    m_documentLoader->setReplacesCurrentHistoryItem(!isNewNavigation);
    UpdateBackForwardListPolicy updateBackForwardList = isNewNavigation && !shouldTreatURLAsSameAsCurrent(url) && !stateObject ? UpdateBackForwardList : DoNotUpdateBackForwardList;
    updateForSameDocumentNavigation(url, SameDocumentNavigationDefault, 0, String(), updateBackForwardList);

    started();

    scrollToFragmentWithParentBoundary(url);

    m_isComplete = false;
    checkCompleted();

    m_frame->document()->statePopped(stateObject ? stateObject : SerializedScriptValue::nullValue());
}
