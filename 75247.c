KURL CSSStyleSheet::completeURL(const String& url) const
{
    if (url.isNull() || m_charset.isEmpty())
        return StyleSheet::completeURL(url);
    const TextEncoding encoding = TextEncoding(m_charset);
    return KURL(baseURL(), url, encoding);
}
