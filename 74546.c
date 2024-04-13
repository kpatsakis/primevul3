PassRefPtr<Widget> FrameLoader::createJavaAppletWidget(const IntSize& size, HTMLAppletElement* element, const HashMap<String, String>& args)
{
    String baseURLString;
    String codeBaseURLString;
    Vector<String> paramNames;
    Vector<String> paramValues;
    HashMap<String, String>::const_iterator end = args.end();
    for (HashMap<String, String>::const_iterator it = args.begin(); it != end; ++it) {
        if (equalIgnoringCase(it->first, "baseurl"))
            baseURLString = it->second;
        else if (equalIgnoringCase(it->first, "codebase"))
            codeBaseURLString = it->second;
        paramNames.append(it->first);
        paramValues.append(it->second);
    }

    if (!codeBaseURLString.isEmpty()) {
        KURL codeBaseURL = completeURL(codeBaseURLString);
        if (!SecurityOrigin::canLoad(codeBaseURL, String(), element->document())) {
            FrameLoader::reportLocalLoadFailed(m_frame, codeBaseURL.string());
            return 0;
        }
    }

    if (baseURLString.isEmpty())
        baseURLString = m_frame->document()->baseURL().string();
    KURL baseURL = completeURL(baseURLString);

    RefPtr<Widget> widget = m_client->createJavaAppletWidget(size, element, baseURL, paramNames, paramValues);
    if (!widget)
        return 0;

    m_containsPlugIns = true;
    return widget;
}
