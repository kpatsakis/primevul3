bool FrameLoader::allAncestorsAreComplete() const
{
    for (Frame* ancestor = m_frame; ancestor; ancestor = ancestor->tree()->parent()) {
        if (!ancestor->document()->loadEventFinished())
            return false;
    }
    return true;
}
