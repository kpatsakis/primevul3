bool ScriptLoader::fetchScript(const String& sourceUrl)
{
    ASSERT(m_element);

    RefPtr<Document> elementDocument(m_element->document());
    if (!m_element->dispatchBeforeLoadEvent(sourceUrl))
        return false;
    if (!m_element->inDocument() || m_element->document() != elementDocument)
        return false;

    ASSERT(!m_resource);
    if (!stripLeadingAndTrailingHTMLSpaces(sourceUrl).isEmpty()) {
        FetchRequest request(ResourceRequest(elementDocument->completeURL(sourceUrl)), m_element->localName());

        AtomicString crossOriginMode = m_element->fastGetAttribute(HTMLNames::crossoriginAttr);
        if (!crossOriginMode.isNull())
            request.setCrossOriginAccessControl(elementDocument->securityOrigin(), crossOriginMode);
        request.setCharset(scriptCharset());

        bool isValidScriptNonce = elementDocument->contentSecurityPolicy()->allowScriptNonce(m_element->fastGetAttribute(HTMLNames::nonceAttr));
        if (isValidScriptNonce)
            request.setContentSecurityCheck(DoNotCheckContentSecurityPolicy);

        m_resource = elementDocument->fetcher()->fetchScript(request);
        m_isExternalScript = true;
    }

    if (m_resource)
        return true;

    dispatchErrorEvent();
    return false;
}
