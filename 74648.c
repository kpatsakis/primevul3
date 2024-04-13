bool FrameLoader::shouldScrollToAnchor(bool isFormSubmission, FrameLoadType loadType, const KURL& url)
{



    return !isFormSubmission
        && loadType != FrameLoadTypeReload
        && loadType != FrameLoadTypeReloadFromOrigin
        && loadType != FrameLoadTypeSame
        && !shouldReload(this->url(), url)
        && !m_frame->document()->isFrameSet();
}
