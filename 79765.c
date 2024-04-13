int HTMLTextAreaElement::maxLength() const
{
    bool ok;
    int value = getAttribute(maxlengthAttr).string().toInt(&ok);
    return ok && value >= 0 ? value : -1;
}
