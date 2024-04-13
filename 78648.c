bool FrameLoader::closeURL()
{
    history()->saveDocumentState();

    if (m_frame->document())
        m_frame->document()->dispatchUnloadEvents();
    stopLoading();

    m_frame->editor().clearUndoRedoOperations();
    return true;
}
