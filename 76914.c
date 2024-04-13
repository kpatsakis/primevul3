void EditorClientBlackBerry::respondToChangedContents()
{
    if (m_webPagePrivate->m_dumpRenderTree)
        m_webPagePrivate->m_dumpRenderTree->didChange();
}
