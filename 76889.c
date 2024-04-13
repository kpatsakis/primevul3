bool EditorClientBlackBerry::canUndo() const
{
    return !m_undoStack.isEmpty();
}
