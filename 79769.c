String HTMLTextAreaElement::sanitizeUserInputValue(const String& proposedValue, unsigned maxLength)
{
    if (maxLength > 0 && U16_IS_LEAD(proposedValue[maxLength - 1]))
        --maxLength;
    return proposedValue.left(maxLength);
}
