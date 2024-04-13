void HTMLFormElement::submit(Event* event, bool activateSubmitButton, bool processingUserGesture, FormSubmissionTrigger formSubmissionTrigger)
{
    FrameView* view = document().view();
    Frame* frame = document().frame();
    if (!view || !frame || !frame->page())
        return;

    if (m_isSubmittingOrPreparingForSubmission) {
        m_shouldSubmit = true;
        return;
    }

    m_isSubmittingOrPreparingForSubmission = true;
    m_wasUserSubmitted = processingUserGesture;

    HTMLFormControlElement* firstSuccessfulSubmitButton = 0;
    bool needButtonActivation = activateSubmitButton; // do we need to activate a submit button?

    for (unsigned i = 0; i < m_associatedElements.size(); ++i) {
        FormAssociatedElement* associatedElement = m_associatedElements[i];
        if (!associatedElement->isFormControlElement())
            continue;
        if (needButtonActivation) {
            HTMLFormControlElement* control = toHTMLFormControlElement(associatedElement);
            if (control->isActivatedSubmit())
                needButtonActivation = false;
            else if (firstSuccessfulSubmitButton == 0 && control->isSuccessfulSubmitButton())
                firstSuccessfulSubmitButton = control;
        }
    }

    if (needButtonActivation && firstSuccessfulSubmitButton)
        firstSuccessfulSubmitButton->setActivatedSubmit(true);

    scheduleFormSubmission(FormSubmission::create(this, m_attributes, event, formSubmissionTrigger));

    if (needButtonActivation && firstSuccessfulSubmitButton)
        firstSuccessfulSubmitButton->setActivatedSubmit(false);

    m_shouldSubmit = false;
    m_isSubmittingOrPreparingForSubmission = false;
}
