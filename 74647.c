bool FrameLoader::shouldReloadToHandleUnreachableURL(DocumentLoader* docLoader)
{
    KURL unreachableURL = docLoader->unreachableURL();

    if (unreachableURL.isEmpty())
        return false;

    if (!isBackForwardLoadType(policyChecker()->loadType()))
        return false;

    DocumentLoader* compareDocumentLoader = 0;
    if (policyChecker()->delegateIsDecidingNavigationPolicy() || policyChecker()->delegateIsHandlingUnimplementablePolicy())
        compareDocumentLoader = m_policyDocumentLoader.get();
    else if (m_delegateIsHandlingProvisionalLoadError)
        compareDocumentLoader = m_provisionalDocumentLoader.get();

    return compareDocumentLoader && unreachableURL == compareDocumentLoader->request().url();
}
