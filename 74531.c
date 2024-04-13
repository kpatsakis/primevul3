void FrameLoader::clear(bool clearWindowProperties, bool clearScriptObjects, bool clearFrameView)
{
    m_frame->editor()->clear();

    if (!m_needsClear)
        return;
    m_needsClear = false;
    
    if (!m_frame->document()->inPageCache()) {
        m_frame->document()->cancelParsing();
        m_frame->document()->stopActiveDOMObjects();
        if (m_frame->document()->attached()) {
            m_frame->document()->willRemove();
            m_frame->document()->detach();
            
            m_frame->document()->removeFocusedNodeOfSubtree(m_frame->document());
        }
    }

    if (clearWindowProperties) {
        m_frame->clearDOMWindow();
        m_frame->script()->clearWindowShell();
    }

    m_frame->selection()->clear();
    m_frame->eventHandler()->clear();
    if (clearFrameView && m_frame->view())
        m_frame->view()->clear();

    m_frame->setSelectionGranularity(CharacterGranularity);

    m_frame->setDocument(0);
    m_decoder = 0;

    m_containsPlugIns = false;

    if (clearScriptObjects)
        m_frame->script()->clearScriptObjects();

    m_frame->redirectScheduler()->clear();

    m_checkTimer.stop();
    m_shouldCallCheckCompleted = false;
    m_shouldCallCheckLoadComplete = false;

    m_receivedData = false;
    m_isDisplayingInitialEmptyDocument = false;

    if (!m_encodingWasChosenByUser)
        m_encoding = String();
}
