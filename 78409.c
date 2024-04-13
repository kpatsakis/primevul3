ScriptValue ScriptController::executeScript(const String& script, bool forceUserGesture)
{
    UserGestureIndicator gestureIndicator(forceUserGesture ? DefinitelyProcessingNewUserGesture : PossiblyProcessingUserGesture);
    return executeScript(ScriptSourceCode(script, m_frame->document()->url()));
}
