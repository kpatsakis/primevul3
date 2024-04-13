bool FrameLoader::isProcessingUserGesture()
{
    Frame* frame = m_frame->tree()->top();
    if (!frame->script()->isEnabled())
        return true; // If JavaScript is disabled, a user gesture must have initiated the navigation.
    return frame->script()->processingUserGesture(); // FIXME: Use pageIsProcessingUserGesture.
}
