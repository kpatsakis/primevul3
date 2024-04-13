void FrameLoader::frameLoadCompleted()
{

    m_client->frameLoadCompleted();

    history()->updateForFrameLoadCompleted();

    if (m_documentLoader)
        m_firstLayoutDone = true;
}
