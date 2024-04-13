CSSStyleSheet::CSSStyleSheet(CSSRule* ownerRule, const String& href, const KURL& baseURL, const String& charset)
    : StyleSheet(ownerRule, href, baseURL)
    , m_namespaces(0)
    , m_charset(charset)
    , m_loadCompleted(false)
    , m_strictParsing(!ownerRule || ownerRule->useStrictParsing())
    , m_hasSyntacticallyValidCSSHeader(true)
{
    CSSStyleSheet* parentSheet = ownerRule ? ownerRule->parentStyleSheet() : 0;
    m_doc = parentSheet ? parentSheet->doc() : 0;
    m_isUserStyleSheet = parentSheet ? parentSheet->isUserStyleSheet() : false;
}
