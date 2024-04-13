String CSSComputedStyleDeclaration::removeProperty(const String&, ExceptionCode& ec)
{
    ec = NO_MODIFICATION_ALLOWED_ERR;
    return String();
}
