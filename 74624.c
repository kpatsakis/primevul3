bool FrameLoader::requestObject(RenderPart* renderer, const String& url, const AtomicString& frameName,
    const String& mimeType, const Vector<String>& paramNames, const Vector<String>& paramValues)
{
    if (url.isEmpty() && mimeType.isEmpty())
        return false;
    
    if (!m_frame->script()->xssAuditor()->canLoadObject(url)) {
        return false;
    }

    KURL completedURL;
    if (!url.isEmpty())
        completedURL = completeURL(url);

    bool useFallback;
    if (shouldUsePlugin(completedURL, mimeType, renderer->hasFallbackContent(), useFallback)) {
        Settings* settings = m_frame->settings();
        if (!settings || !settings->arePluginsEnabled() || 
            (!settings->isJavaEnabled() && MIMETypeRegistry::isJavaAppletMIMEType(mimeType)))
            return false;
        return loadPlugin(renderer, completedURL, mimeType, paramNames, paramValues, useFallback);
    }

    ASSERT(renderer->node()->hasTagName(objectTag) || renderer->node()->hasTagName(embedTag));
    HTMLPlugInElement* element = static_cast<HTMLPlugInElement*>(renderer->node());
    
    return loadSubframe(element, completedURL, frameName, m_outgoingReferrer);
}
