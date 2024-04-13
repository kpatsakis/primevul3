void FrameLoader::scheduleCheckLoadComplete()
{
    m_shouldCallCheckLoadComplete = true;
    startCheckCompleteTimer();
}
