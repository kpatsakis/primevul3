bool CSSStyleSheet::parseString(const String &string, bool strict)
{
    setStrictParsing(strict);
    CSSParser p(strict);
    p.parseSheet(this, string);
    return true;
}
