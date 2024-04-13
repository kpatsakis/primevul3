static void resourceInfo(const v8::Handle<v8::Function> function, String& resourceName, int& lineNumber)
{
    v8::ScriptOrigin origin = function->GetScriptOrigin();
    if (origin.ResourceName().IsEmpty()) {
        resourceName = "undefined";
        lineNumber = 1;
    } else {
        resourceName = toWebCoreString(origin.ResourceName());
        lineNumber = function->GetScriptLineNumber() + 1;
    }
}
