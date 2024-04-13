bool FrameLoader::shouldUsePlugin(const KURL& url, const String& mimeType, bool hasFallback, bool& useFallback)
{
    if (m_client->shouldUsePluginDocument(mimeType)) {
        useFallback = false;
        return true;
    }

    if (m_frame->page() && (mimeType == "image/tiff" || mimeType == "image/tif" || mimeType == "image/x-tiff")) {
        const PluginData* pluginData = m_frame->page()->pluginData();
        String pluginName = pluginData ? pluginData->pluginNameForMimeType(mimeType) : String();
        if (!pluginName.isEmpty() && !pluginName.contains("QuickTime", false)) 
            return true;
    }
        
    ObjectContentType objectType = m_client->objectContentType(url, mimeType);
    useFallback = objectType == ObjectContentNone && hasFallback;
    return objectType == ObjectContentNone || objectType == ObjectContentNetscapePlugin || objectType == ObjectContentOtherPlugin;
}
