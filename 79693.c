void HTMLInputElement::setSize(unsigned size, ExceptionState& exceptionState)
{
    if (!size)
        exceptionState.throwDOMException(IndexSizeError, "The value provided is 0, which is an invalid size.");
    else
        setSize(size);
}
