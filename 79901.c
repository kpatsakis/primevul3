void ScriptLoader::notifyFinished(Resource* resource)
{
    ASSERT(!m_willBeParserExecuted);

    RefPtr<Document> elementDocument(m_element->document());
    RefPtr<Document> contextDocument = elementDocument->contextDocument().get();
    if (!contextDocument)
        return;

    ASSERT_UNUSED(resource, resource == m_resource);
    if (!m_resource)
        return;
    if (m_resource->errorOccurred()) {
        dispatchErrorEvent();
        contextDocument->scriptRunner()->notifyScriptLoadError(this, m_willExecuteInOrder ? ScriptRunner::IN_ORDER_EXECUTION : ScriptRunner::ASYNC_EXECUTION);
        return;
    }
    if (m_willExecuteInOrder)
        contextDocument->scriptRunner()->notifyScriptReady(this, ScriptRunner::IN_ORDER_EXECUTION);
    else
        contextDocument->scriptRunner()->notifyScriptReady(this, ScriptRunner::ASYNC_EXECUTION);

    m_resource = 0;
}
