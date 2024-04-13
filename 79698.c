void HTMLInputElement::setValueAsNumber(double newValue, ExceptionState& exceptionState, TextFieldEventBehavior eventBehavior)
{
    if (!std::isfinite(newValue)) {
        exceptionState.throwDOMException(NotSupportedError, ExceptionMessages::notAFiniteNumber(newValue));
        return;
    }
    m_inputType->setValueAsDouble(newValue, eventBehavior, exceptionState);
}
