void EditorClientBlackBerry::registerUndoStep(PassRefPtr<UndoStep> step)
{
    if (m_undoStack.size() == maximumUndoStackDepth)
        m_undoStack.removeFirst();

    if (!m_inRedo)
        m_redoStack.clear();

    m_undoStack.append(step);
}
