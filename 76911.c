void EditorClientBlackBerry::registerRedoStep(PassRefPtr<UndoStep> step)
{
    m_redoStack.append(step);
}
