void HTMLInputElement::setSelectionEndForBinding(int end, ExceptionState& exceptionState)
{
    if (!m_inputType->supportsSelectionAPI()) {
        exceptionState.throwDOMException(InvalidStateError, "The input element's type ('" + m_inputType->formControlType() + "') does not support selection.");
        return;
    }
    HTMLTextFormControlElement::setSelectionEnd(end);
}
