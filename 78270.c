void HTMLFormElement::submitFromJavaScript()
{
    submit(0, false, UserGestureIndicator::processingUserGesture(), SubmittedByJavaScript);
}
