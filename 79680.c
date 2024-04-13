int HTMLInputElement::selectionEndForBinding(ExceptionState& exceptionState) const
{
    if (!m_inputType->supportsSelectionAPI()) {
        exceptionState.throwDOMException(InvalidStateError, "The input element's type ('" + m_inputType->formControlType() + "') does not support selection.");
        return 0;
    }
    return HTMLTextFormControlElement::selectionEnd();
}
