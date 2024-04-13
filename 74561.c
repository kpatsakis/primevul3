Frame* FrameLoader::findFrameForNavigation(const AtomicString& name)
{
    Frame* frame = m_frame->tree()->find(name);
    if (!shouldAllowNavigation(frame))
        return 0;  
    return frame;
}
