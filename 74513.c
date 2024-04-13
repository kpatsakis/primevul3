void FrameLoader::callContinueLoadAfterNewWindowPolicy(void* argument,
    const ResourceRequest& request, PassRefPtr<FormState> formState, const String& frameName, bool shouldContinue)
{
    FrameLoader* loader = static_cast<FrameLoader*>(argument);
    loader->continueLoadAfterNewWindowPolicy(request, formState, frameName, shouldContinue);
}
