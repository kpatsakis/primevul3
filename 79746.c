String HTMLSelectElement::validationMessage() const
{
    if (!willValidate())
        return String();
    if (customError())
        return customValidationMessage();
    if (valueMissing())
        return locale().queryString(blink::WebLocalizedString::ValidationValueMissingForSelect);
    return String();
}
