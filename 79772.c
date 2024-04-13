void HTMLTextAreaElement::setMaxLength(int newValue, ExceptionState& exceptionState)
{
    if (newValue < 0)
        exceptionState.throwDOMException(IndexSizeError, "The value provided (" + String::number(newValue) + ") is not positive or 0.");
    else
        setIntegralAttribute(maxlengthAttr, newValue);
}
