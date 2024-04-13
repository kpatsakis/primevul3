bool FrameLoader::shouldClose()
{
    Page* page = m_frame->page();
    if (!page || !page->chrome().canRunBeforeUnloadConfirmPanel())
        return true;

    Vector<RefPtr<Frame> > targetFrames;
    targetFrames.append(m_frame);
    for (Frame* child = m_frame->tree()->firstChild(); child; child = child->tree()->traverseNext(m_frame))
        targetFrames.append(child);

    bool shouldClose = false;
    {
        NavigationDisablerForBeforeUnload navigationDisabler;
        size_t i;

        for (i = 0; i < targetFrames.size(); i++) {
            if (!targetFrames[i]->tree()->isDescendantOf(m_frame))
                continue;
            if (!targetFrames[i]->document()->dispatchBeforeUnloadEvent(page->chrome(), m_frame->document()))
                break;
        }

        if (i == targetFrames.size())
            shouldClose = true;
    }
    return shouldClose;
}
