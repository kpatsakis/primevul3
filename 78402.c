static inline v8::Local<v8::Context> contextForWorld(ScriptController* scriptController, DOMWrapperWorld* world)
{
    return scriptController->windowShell(world)->context();
}
