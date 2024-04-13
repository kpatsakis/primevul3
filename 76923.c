bool EditorClientBlackBerry::shouldInsertText(const WTF::String& text, Range* range, EditorInsertAction insertAction)
{
    if (m_webPagePrivate->m_dumpRenderTree)
        return m_webPagePrivate->m_dumpRenderTree->shouldInsertText(text, range, static_cast<int>(insertAction));
    return true;
}
