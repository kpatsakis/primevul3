bool HTMLTextAreaElement::tooLong() const
{
    return willValidate() && tooLong(value(), CheckDirtyFlag);
}
