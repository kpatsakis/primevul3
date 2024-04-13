void ScriptController::namedItemAdded(HTMLDocument* doc, const AtomicString& name)
{
    windowShell(mainThreadNormalWorld())->namedItemAdded(doc, name);
}
