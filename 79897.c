bool ScriptLoader::ignoresLoadRequest() const
{
    return m_alreadyStarted || m_isExternalScript || m_parserInserted || !element() || !element()->inDocument();
}
