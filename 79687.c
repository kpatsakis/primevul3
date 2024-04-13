void HTMLInputElement::setRangeText(const String& replacement, unsigned start, unsigned end, const String& selectionMode, ExceptionState& exceptionState)
{
    if (!m_inputType->supportsSelectionAPI()) {
        exceptionState.throwDOMException(InvalidStateError, "The input element's type ('" + m_inputType->formControlType() + "') does not support selection.");
        return;
    }

    HTMLTextFormControlElement::setRangeText(replacement, start, end, selectionMode, exceptionState);
}
