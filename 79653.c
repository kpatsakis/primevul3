bool HTMLInputElement::capture() const
{
    if (!isFileUpload() || !fastHasAttribute(captureAttr))
        return false;

    bool hasDeprecatedUsage = !fastGetAttribute(captureAttr).isNull();
    if (hasDeprecatedUsage)
        UseCounter::countDeprecation(document(), UseCounter::CaptureAttributeAsEnum);
    else
        UseCounter::count(document(), UseCounter::CaptureAttributeAsEnum);

    return true;
}
