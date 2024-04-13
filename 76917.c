void EditorClientBlackBerry::setInputMethodState(bool)
{
    m_webPagePrivate->m_inputHandler->focusedNodeChanged();
}
