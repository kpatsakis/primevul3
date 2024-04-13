void FrameLoader::setState(FrameState newState)
{    
    m_state = newState;
    
    if (newState == FrameStateProvisional)
        provisionalLoadStarted();
    else if (newState == FrameStateComplete) {
        frameLoadCompleted();
        storedTimeOfLastCompletedLoad = currentTime();
        if (m_documentLoader)
            m_documentLoader->stopRecordingResponses();
    }
}
