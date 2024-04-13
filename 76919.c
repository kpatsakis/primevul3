bool EditorClientBlackBerry::shouldDeleteRange(Range* range)
{
    if (m_webPagePrivate->m_dumpRenderTree)
        return m_webPagePrivate->m_dumpRenderTree->shouldDeleteDOMRange(range);
    return true;
}
