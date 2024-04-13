void FrameLoader::stopLoading()
{
    m_isComplete = true; // to avoid calling completed() in finishedParsing()

    if (m_frame->document() && m_frame->document()->parsing()) {
        finishedParsing();
        m_frame->document()->setParsing(false);
    }

    if (Document* doc = m_frame->document()) {
        doc->setReadyState(Document::Complete);

        DatabaseManager::manager().stopDatabases(doc, 0);
    }

    m_frame->navigationScheduler()->cancel();
}
