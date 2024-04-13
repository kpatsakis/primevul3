void FrameLoader::init()
{
    m_provisionalDocumentLoader = m_client->createDocumentLoader(ResourceRequest(KURL(ParsedURLString, emptyString())), SubstituteData());
    m_provisionalDocumentLoader->setFrame(m_frame);
    m_provisionalDocumentLoader->startLoadingMainResource();
    m_frame->document()->cancelParsing();
    m_stateMachine.advanceTo(FrameLoaderStateMachine::DisplayingInitialEmptyDocument);
    m_progressTracker = FrameProgressTracker::create(m_frame);
}
