V8Extensions& ScriptController::registeredExtensions()
{
    DEFINE_STATIC_LOCAL(V8Extensions, extensions, ());
    return extensions;
}
