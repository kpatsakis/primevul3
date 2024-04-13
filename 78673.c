void FrameLoader::load(const FrameLoadRequest& passedRequest)
{
    ASSERT(!m_suppressOpenerInNewFrame);
    ASSERT(m_frame->document());

    RefPtr<Frame> protect(m_frame);

    if (m_inStopAllLoaders)
        return;

    FrameLoadRequest request(passedRequest);
    if (!prepareRequestForThisFrame(request))
        return;

    Frame* targetFrame = findFrameForNavigation(request.frameName(), request.formState() ? request.formState()->sourceDocument() : m_frame->document());
    if (targetFrame && targetFrame != m_frame) {
        request.setFrameName("_self");
        targetFrame->loader()->load(request);
        return;
    }

    FrameLoadType newLoadType = determineFrameLoadType(request);
    NavigationAction action(request.resourceRequest(), newLoadType, request.formState(), request.triggeringEvent());
    if (shouldOpenInNewWindow(targetFrame, request, action)) {
        TemporaryChange<bool> changeOpener(m_suppressOpenerInNewFrame, request.shouldSendReferrer() == NeverSendReferrer);
        checkNewWindowPolicyAndContinue(request.formState(), request.frameName(), action);
        return;
    }

    TemporaryChange<bool> changeClientRedirect(m_startingClientRedirect, request.clientRedirect());
    if (shouldPerformFragmentNavigation(request.formState(), request.resourceRequest().httpMethod(), newLoadType, request.resourceRequest().url())) {
        checkNavigationPolicyAndContinueFragmentScroll(action, newLoadType != FrameLoadTypeRedirectWithLockedBackForwardList);
        return;
    }
    bool sameURL = shouldTreatURLAsSameAsCurrent(request.resourceRequest().url());
    loadWithNavigationAction(request.resourceRequest(), action, newLoadType, request.formState(), request.substituteData());
    if (sameURL && newLoadType != FrameLoadTypeReload && newLoadType != FrameLoadTypeReloadFromOrigin && request.resourceRequest().httpMethod() != "POST")
        m_loadType = FrameLoadTypeSame;
}
