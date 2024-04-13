void ScriptLoader::stopLoadRequest()
{
    if (m_resource) {
        if (!m_willBeParserExecuted)
            m_resource->removeClient(this);
        m_resource = 0;
    }
}
