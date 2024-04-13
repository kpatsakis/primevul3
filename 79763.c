bool HTMLTextAreaElement::isValidValue(const String& candidate) const
{
    return !valueMissing(candidate) && !tooLong(candidate, IgnoreDirtyFlag);
}
