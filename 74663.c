static HTMLPlugInElement* toPlugInElement(Node* node)
{
    if (!node)
        return 0;

#if ENABLE(PLUGIN_PROXY_FOR_VIDEO)
    ASSERT(node->hasTagName(objectTag) || node->hasTagName(embedTag) 
        || node->hasTagName(videoTag) || node->hasTagName(audioTag)
        || node->hasTagName(appletTag));
#else
    ASSERT(node->hasTagName(objectTag) || node->hasTagName(embedTag) 
        || node->hasTagName(appletTag));
#endif

    return static_cast<HTMLPlugInElement*>(node);
}
