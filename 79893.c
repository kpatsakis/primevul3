void ScriptLoader::execute(ScriptResource* resource)
{
    ASSERT(!m_willBeParserExecuted);
    ASSERT(resource);
    if (resource->errorOccurred()) {
        dispatchErrorEvent();
    } else if (!resource->wasCanceled()) {
        executeScript(ScriptSourceCode(resource));
        dispatchLoadEvent();
    }
    resource->removeClient(this);
}
