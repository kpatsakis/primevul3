void EditorClientBlackBerry::respondToChangedSelection(Frame* frame)
{
    if (m_waitingForCursorFocus)
        m_waitingForCursorFocus = false;
    else
        m_webPagePrivate->selectionChanged(frame);

    if (m_webPagePrivate->m_dumpRenderTree)
        m_webPagePrivate->m_dumpRenderTree->didChangeSelection();
}
