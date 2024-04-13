void ScriptLoader::didNotifySubtreeInsertionsToDocument()
{
    if (!m_parserInserted)
        prepareScript(); // FIXME: Provide a real starting line number here.
}
