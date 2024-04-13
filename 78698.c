bool FrameLoader::shouldPerformFragmentNavigation(bool isFormSubmission, const String& httpMethod, FrameLoadType loadType, const KURL& url)
{
    ASSERT(loadType != FrameLoadTypeBackForward);
    ASSERT(loadType != FrameLoadTypeReloadFromOrigin);
    return (!isFormSubmission || equalIgnoringCase(httpMethod, "GET"))
        && loadType != FrameLoadTypeReload
        && loadType != FrameLoadTypeSame
        && url.hasFragmentIdentifier()
        && equalIgnoringFragmentIdentifier(m_frame->document()->url(), url)
        && !m_frame->document()->isFrameSet();
}
