FrameLoadType FrameLoader::determineFrameLoadType(const FrameLoadRequest& request)
{
    if (m_frame->tree()->parent() && !m_stateMachine.startedFirstRealLoad())
        return FrameLoadTypeInitialInChildFrame;
    if (request.resourceRequest().cachePolicy() == ReloadIgnoringCacheData)
        return FrameLoadTypeReload;
    if (request.lockBackForwardList() || isScriptTriggeredFormSubmissionInChildFrame(request))
        return FrameLoadTypeRedirectWithLockedBackForwardList;
    if (!request.requester() && shouldTreatURLAsSameAsCurrent(request.resourceRequest().url()))
        return FrameLoadTypeSame;
    if (shouldTreatURLAsSameAsCurrent(request.substituteData().failingURL()) && m_loadType == FrameLoadTypeReload)
        return FrameLoadTypeReload;
    return FrameLoadTypeStandard;
}
