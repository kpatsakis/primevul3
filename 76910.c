void EditorClientBlackBerry::redo()
{
    if (canRedo()) {
        EditCommandStack::iterator back = --m_redoStack.end();
        RefPtr<UndoStep> command(*back);
        m_redoStack.remove(back);

        ASSERT(!m_inRedo);
        m_inRedo = true;

        command->reapply();
        m_inRedo = false;
    }
}
