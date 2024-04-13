void HTMLInputElement::setSelectionRangeForBinding(int start, int end, const String& direction, ExceptionState& exceptionState)
{
    if (!m_inputType->supportsSelectionAPI()) {
        exceptionState.throwDOMException(InvalidStateError, "The input element's type ('" + m_inputType->formControlType() + "') does not support selection.");
        return;
    }
    HTMLTextFormControlElement::setSelectionRange(start, end, direction);
}
