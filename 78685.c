void FrameLoader::receivedFirstData()
{
    if (m_stateMachine.creatingInitialEmptyDocument())
        return;
    m_client->dispatchDidCommitLoad();
    InspectorInstrumentation::didCommitLoad(m_frame, m_documentLoader.get());
    m_frame->page()->didCommitLoad(m_frame);
    dispatchDidClearWindowObjectsInAllWorlds();
}
