bool EditorClientBlackBerry::shouldEndEditing(Range* range)
{
    if (m_webPagePrivate->m_dumpRenderTree)
        return m_webPagePrivate->m_dumpRenderTree->shouldEndEditingInDOMRange(range);
    return true;
}
