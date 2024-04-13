bool EditorClientBlackBerry::isSelectTrailingWhitespaceEnabled()
{
    if (m_webPagePrivate->m_dumpRenderTree)
        return m_webPagePrivate->m_dumpRenderTree->isSelectTrailingWhitespaceEnabled();
    return false;
}
