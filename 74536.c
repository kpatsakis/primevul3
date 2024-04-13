bool FrameLoader::closeURL()
{
    history()->saveDocumentState();
    
    Document* currentDocument = m_frame->document();
    stopLoading(currentDocument && !currentDocument->inPageCache() ? UnloadEventPolicyUnloadAndPageHide : UnloadEventPolicyUnloadOnly);
    
    m_frame->editor()->clearUndoRedoOperations();
    return true;
}
