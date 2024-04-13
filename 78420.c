bool ScriptController::shouldBypassMainWorldContentSecurityPolicy()
{
    if (DOMWrapperWorld* world = isolatedWorldForEnteredContext())
        return world->isolatedWorldHasContentSecurityPolicy();
    return false;
}
