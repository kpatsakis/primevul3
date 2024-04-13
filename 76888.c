bool EditorClientBlackBerry::canRedo() const
{
    return !m_redoStack.isEmpty();
}
