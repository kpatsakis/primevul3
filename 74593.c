void FrameLoader::loadPostRequest(const ResourceRequest& inRequest, const String& referrer, const String& frameName, bool lockHistory, FrameLoadType loadType, PassRefPtr<Event> event, PassRefPtr<FormState> prpFormState)
{
    RefPtr<FormState> formState = prpFormState;



    const KURL& url = inRequest.url();
    RefPtr<FormData> formData = inRequest.httpBody();
    const String& contentType = inRequest.httpContentType();
    String origin = inRequest.httpOrigin();

    ResourceRequest workingResourceRequest(url);    

    if (!referrer.isEmpty())
        workingResourceRequest.setHTTPReferrer(referrer);
    workingResourceRequest.setHTTPOrigin(origin);
    workingResourceRequest.setHTTPMethod("POST");
    workingResourceRequest.setHTTPBody(formData);
    workingResourceRequest.setHTTPContentType(contentType);
    addExtraFieldsToRequest(workingResourceRequest, loadType, true, true);

    NavigationAction action(url, loadType, true, event);

    if (!frameName.isEmpty()) {
        if (Frame* targetFrame = formState ? 0 : findFrameForNavigation(frameName))
            targetFrame->loader()->loadWithNavigationAction(workingResourceRequest, action, lockHistory, loadType, formState.release());
        else
            policyChecker()->checkNewWindowPolicy(action, FrameLoader::callContinueLoadAfterNewWindowPolicy, workingResourceRequest, formState.release(), frameName, this);
    } else
        loadWithNavigationAction(workingResourceRequest, action, lockHistory, loadType, formState.release());    
}
