void FrameLoader::provisionalLoadStarted()
{    
    m_firstLayoutDone = false;
    m_frame->redirectScheduler()->cancel(true);
    m_client->provisionalLoadStarted();
}
