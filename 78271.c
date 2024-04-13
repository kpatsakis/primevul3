void HTMLFormElement::submitImplicitly(Event* event, bool fromImplicitSubmissionTrigger)
{
    int submissionTriggerCount = 0;
    for (unsigned i = 0; i < m_associatedElements.size(); ++i) {
        FormAssociatedElement* formAssociatedElement = m_associatedElements[i];
        if (!formAssociatedElement->isFormControlElement())
            continue;
        HTMLFormControlElement* control = toHTMLFormControlElement(formAssociatedElement);
        if (control->isSuccessfulSubmitButton()) {
            if (control->renderer()) {
                control->dispatchSimulatedClick(event);
                return;
            }
        } else if (control->canTriggerImplicitSubmission())
            ++submissionTriggerCount;
    }
    if (fromImplicitSubmissionTrigger && submissionTriggerCount == 1)
        prepareForSubmission(event);
}
