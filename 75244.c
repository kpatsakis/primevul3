int CSSStyleSheet::addRule(const String& selector, const String& style, ExceptionCode& ec)
{
     return addRule(selector, style, length(), ec);
 }
