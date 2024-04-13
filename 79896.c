void ScriptLoader::handleSourceAttribute(const String& sourceUrl)
{
    if (ignoresLoadRequest() || sourceUrl.isEmpty())
        return;

    prepareScript(); // FIXME: Provide a real starting line number here.
}
