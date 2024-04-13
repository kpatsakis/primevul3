void FrameLoader::frameDetached()
{
    RefPtr<Frame> protect(m_frame);
    stopAllLoaders();
    m_frame->document()->stopActiveDOMObjects();
    detachFromParent();
}
