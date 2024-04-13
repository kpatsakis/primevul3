bool FrameLoader::isScriptTriggeredFormSubmissionInChildFrame(const FrameLoadRequest& request) const
{
    if (!m_frame->tree()->parent() || ScriptController::processingUserGesture())
        return false;
    return request.formState() && request.formState()->formSubmissionTrigger() == SubmittedByJavaScript;
}
