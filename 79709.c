bool HTMLInputElement::tooLong(const String& value, NeedsToCheckDirtyFlag check) const
{
    if (!isTextType())
        return false;
    int max = maxLength();
    if (max < 0)
        return false;
    if (check == CheckDirtyFlag) {
        if (!hasDirtyValue() || !lastChangeWasUserEdit())
            return false;
    }
    return value.length() > static_cast<unsigned>(max);
}
