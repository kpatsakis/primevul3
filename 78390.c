ScriptController::ScriptController(Frame* frame)
    : m_frame(frame)
    , m_sourceURL(0)
    , m_isolate(v8::Isolate::GetCurrent())
    , m_windowShell(V8WindowShell::create(frame, mainThreadNormalWorld(), m_isolate))
    , m_paused(false)
    , m_windowScriptNPObject(0)
{
}
