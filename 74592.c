bool FrameLoader::loadPlugin(RenderPart* renderer, const KURL& url, const String& mimeType, 
    const Vector<String>& paramNames, const Vector<String>& paramValues, bool useFallback)
{
    RefPtr<Widget> widget;

    if (renderer && !useFallback) {
        HTMLPlugInElement* element = toPlugInElement(renderer->node());

        if (!SecurityOrigin::canLoad(url, String(), frame()->document())) {
            FrameLoader::reportLocalLoadFailed(m_frame, url.string());
            return false;
        }

        checkIfRunInsecureContent(m_frame->document()->securityOrigin(), url);

        widget = m_client->createPlugin(IntSize(renderer->contentWidth(), renderer->contentHeight()),
                                        element, url, paramNames, paramValues, mimeType,
                                        m_frame->document()->isPluginDocument() && !m_containsPlugIns);
        if (widget) {
            renderer->setWidget(widget);
            m_containsPlugIns = true;
        }
    }

    return widget != 0;
}
