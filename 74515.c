bool FrameLoader::canCachePage()
{
#ifndef NDEBUG
    logCanCachePageDecision();
#endif
    
    FrameLoadType loadType = this->loadType();
    
    return !m_frame->tree()->parent()
        && canCachePageContainingThisFrame()
        && m_frame->page()
        && m_frame->page()->backForwardList()->enabled()
        && m_frame->page()->backForwardList()->capacity() > 0
        && m_frame->page()->settings()->usesPageCache()
        && loadType != FrameLoadTypeReload 
        && loadType != FrameLoadTypeReloadFromOrigin
        && loadType != FrameLoadTypeSame
        ;
}
