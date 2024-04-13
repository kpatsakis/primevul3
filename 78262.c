void HTMLFormElement::scheduleFormSubmission(PassRefPtr<FormSubmission> submission)
{
    ASSERT(submission->method() == FormSubmission::PostMethod || submission->method() == FormSubmission::GetMethod);
    ASSERT(submission->data());
    ASSERT(submission->state());
    if (submission->action().isEmpty())
        return;
    if (document().isSandboxed(SandboxForms)) {
        document().addConsoleMessage(SecurityMessageSource, ErrorMessageLevel, "Blocked form submission to '" + submission->action().elidedString() + "' because the form's frame is sandboxed and the 'allow-forms' permission is not set.");
        return;
    }

    if (protocolIsJavaScript(submission->action())) {
        if (!document().contentSecurityPolicy()->allowFormAction(KURL(submission->action())))
            return;
        document().frame()->script()->executeScriptIfJavaScriptURL(submission->action());
        return;
    }
    submission->setReferrer(document().frame()->loader()->outgoingReferrer());
    submission->setOrigin(document().frame()->loader()->outgoingOrigin());

    document().frame()->navigationScheduler()->scheduleFormSubmission(submission);
}
