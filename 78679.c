int FrameLoader::numPendingOrLoadingRequests(bool recurse) const
{
    if (!recurse)
        return m_frame->document()->fetcher()->requestCount();

    int count = 0;
    for (Frame* frame = m_frame; frame; frame = frame->tree()->traverseNext(m_frame))
        count += frame->document()->fetcher()->requestCount();
    return count;
}
