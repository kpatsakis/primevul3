SandboxFlags FrameLoader::effectiveSandboxFlags() const
{
    SandboxFlags flags = m_forcedSandboxFlags;
    if (Frame* parentFrame = m_frame->tree()->parent())
        flags |= parentFrame->document()->sandboxFlags();
    if (HTMLFrameOwnerElement* ownerElement = m_frame->ownerElement())
        flags |= ownerElement->sandboxFlags();
    return flags;
}
