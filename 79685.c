void HTMLInputElement::setMaxLength(int maxLength, ExceptionState& exceptionState)
{
    if (maxLength < 0)
        exceptionState.throwDOMException(IndexSizeError, "The value provided (" + String::number(maxLength) + ") is negative.");
    else
        setIntegralAttribute(maxlengthAttr, maxLength);
}
