bool HTMLTextAreaElement::tooLong(const String& value, NeedsToCheckDirtyFlag check) const
{
    if (check == CheckDirtyFlag && !lastChangeWasUserEdit())
        return false;

    int max = maxLength();
    if (max < 0)
        return false;
    return computeLengthForSubmission(value) > static_cast<unsigned>(max);
}
