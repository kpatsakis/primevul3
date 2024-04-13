void FrameLoader::stopForUserCancel(bool deferCheckLoadComplete)
{
    stopAllLoaders();
    
    if (deferCheckLoadComplete)
        scheduleCheckLoadComplete();
    else if (m_frame->page())
        checkLoadComplete();
}
