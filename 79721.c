bool HTMLSelectElement::anonymousIndexedSetter(unsigned index, PassRefPtr<HTMLOptionElement> value, ExceptionState& exceptionState)
{
    if (!value) {
        exceptionState.throwTypeError(ExceptionMessages::failedToSet(String::number(index), "HTMLSelectElement", "The value provided was not an HTMLOptionElement."));
        return false;
    }
    setOption(index, value.get(), exceptionState);
    return true;
}
