void FrameLoader::scheduleCheckCompleted()
{
    m_shouldCallCheckCompleted = true;
    startCheckCompleteTimer();
}
