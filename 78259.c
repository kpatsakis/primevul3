void HTMLFormElement::requestAutocomplete()
{
    Frame* frame = document().frame();
    if (!frame)
        return;

    if (!shouldAutocomplete() || !UserGestureIndicator::processingUserGesture()) {
        finishRequestAutocomplete(AutocompleteResultErrorDisabled);
        return;
    }

    StringPairVector controlNamesAndValues;
    getTextFieldValues(controlNamesAndValues);
    RefPtr<FormState> formState = FormState::create(this, controlNamesAndValues, &document(), SubmittedByJavaScript);
    frame->loader()->client()->didRequestAutocomplete(formState.release());
}
