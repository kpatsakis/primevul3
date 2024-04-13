Frame* FrameLoader::findFrameForNavigation(const AtomicString& name, Document* activeDocument)
{
    ASSERT(activeDocument);
    Frame* frame = m_frame->tree()->find(name);

    if (frame == m_frame && name != "_self" && m_frame->document()->shouldDisplaySeamlesslyWithParent()) {
        for (Frame* ancestor = m_frame; ancestor; ancestor = ancestor->tree()->parent()) {
            if (!ancestor->document()->shouldDisplaySeamlesslyWithParent()) {
                frame = ancestor;
                break;
            }
        }
        ASSERT(frame != m_frame);
    }

    if (!activeDocument->canNavigate(frame))
        return 0;
    return frame;
}
