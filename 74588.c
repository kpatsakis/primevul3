void FrameLoader::load(DocumentLoader* newDocumentLoader)
{
    ResourceRequest& r = newDocumentLoader->request();
    addExtraFieldsToMainResourceRequest(r);
    FrameLoadType type;

    if (shouldTreatURLAsSameAsCurrent(newDocumentLoader->originalRequest().url())) {
        r.setCachePolicy(ReloadIgnoringCacheData);
        type = FrameLoadTypeSame;
    } else
        type = FrameLoadTypeStandard;

    if (m_documentLoader)
        newDocumentLoader->setOverrideEncoding(m_documentLoader->overrideEncoding());
    
    if (shouldReloadToHandleUnreachableURL(newDocumentLoader)) {
        ASSERT(type == FrameLoadTypeStandard);
        type = FrameLoadTypeReload;
    }

    loadWithDocumentLoader(newDocumentLoader, type, 0);
}
