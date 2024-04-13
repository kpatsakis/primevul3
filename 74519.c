void FrameLoader::cancelAndClear()
{
    m_frame->redirectScheduler()->cancel();

    if (!m_isComplete)
        closeURL();

    clear(false);
    m_frame->script()->updatePlatformScriptObjects();
}
