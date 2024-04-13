void ScriptController::namedItemRemoved(HTMLDocument* doc, const AtomicString& name)
{
    windowShell(mainThreadNormalWorld())->namedItemRemoved(doc, name);
}
