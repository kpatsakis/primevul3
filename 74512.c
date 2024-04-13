void FrameLoader::callContinueLoadAfterNavigationPolicy(void* argument,
    const ResourceRequest& request, PassRefPtr<FormState> formState, bool shouldContinue)
{
    FrameLoader* loader = static_cast<FrameLoader*>(argument);
    loader->continueLoadAfterNavigationPolicy(request, formState, shouldContinue);
}
