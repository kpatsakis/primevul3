void FrameLoader::loadWithDocumentLoader(DocumentLoader* loader, FrameLoadType type, PassRefPtr<FormState> prpFormState)
{
    ASSERT(m_client->hasWebView());


    ASSERT(m_frame->view());

    if (m_unloadEventBeingDispatched)
        return;

    policyChecker()->setLoadType(type);
    RefPtr<FormState> formState = prpFormState;
    bool isFormSubmission = formState;

    const KURL& newURL = loader->request().url();

    if (shouldScrollToAnchor(isFormSubmission, policyChecker()->loadType(), newURL)) {
        RefPtr<DocumentLoader> oldDocumentLoader = m_documentLoader;
        NavigationAction action(newURL, policyChecker()->loadType(), isFormSubmission);

        oldDocumentLoader->setTriggeringAction(action);
        policyChecker()->stopCheck();
        policyChecker()->checkNavigationPolicy(loader->request(), oldDocumentLoader.get(), formState,
            callContinueFragmentScrollAfterNavigationPolicy, this);
    } else {
        if (Frame* parent = m_frame->tree()->parent())
            loader->setOverrideEncoding(parent->loader()->documentLoader()->overrideEncoding());

        policyChecker()->stopCheck();
        setPolicyDocumentLoader(loader);
        if (loader->triggeringAction().isEmpty())
            loader->setTriggeringAction(NavigationAction(newURL, policyChecker()->loadType(), isFormSubmission));

        if (Element* ownerElement = m_frame->document()->ownerElement()) {
            if (!ownerElement->dispatchBeforeLoadEvent(loader->request().url().string())) {
                continueLoadAfterNavigationPolicy(loader->request(), formState, false);
                return;
            }
        }

        policyChecker()->checkNavigationPolicy(loader->request(), loader, formState,
            callContinueLoadAfterNavigationPolicy, this);
    }
}
