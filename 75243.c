int CSSStyleSheet::addRule(const String& selector, const String& style, int index, ExceptionCode& ec)
{
    insertRule(selector + " { " + style + " }", index, ec);

    return -1;
}
