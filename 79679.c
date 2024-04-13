String HTMLInputElement::selectionDirectionForBinding(ExceptionState& exceptionState) const
{
    if (!m_inputType->supportsSelectionAPI()) {
        exceptionState.throwDOMException(InvalidStateError, "The input element's type ('" + m_inputType->formControlType() + "') does not support selection.");
        return String();
    }
    return HTMLTextFormControlElement::selectionDirection();
}
