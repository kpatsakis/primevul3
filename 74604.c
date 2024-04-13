int FrameLoader::numPendingOrLoadingRequests(bool recurse) const
{
    if (!recurse)
        return numRequests(m_frame->document());

    int count = 0;
    for (Frame* frame = m_frame; frame; frame = frame->tree()->traverseNext(m_frame))
        count += numRequests(frame->document());
    return count;
}
