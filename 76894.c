void EditorClientBlackBerry::didBeginEditing()
{
    if (m_webPagePrivate->m_dumpRenderTree)
        m_webPagePrivate->m_dumpRenderTree->didBeginEditing();
}
