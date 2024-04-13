void FrameLoader::frameDetached()
{
    stopAllLoaders();
    m_frame->document()->stopActiveDOMObjects();
    detachFromParent();
}
