String HTMLFormControlElement::nameForAutofill() const
{
    String fullName = name();
    String trimmedName = fullName.stripWhiteSpace();
    if (!trimmedName.isEmpty())
        return trimmedName;
    fullName = getIdAttribute();
    trimmedName = fullName.stripWhiteSpace();
    return trimmedName;
}
