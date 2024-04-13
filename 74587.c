void FrameLoader::load(const ResourceRequest& request, const String& frameName, bool lockHistory)
{
    if (frameName.isEmpty()) {
        load(request, lockHistory);
        return;
    }

    Frame* frame = findFrameForNavigation(frameName);
    if (frame) {
        frame->loader()->load(request, lockHistory);
        return;
    }

    policyChecker()->checkNewWindowPolicy(NavigationAction(request.url(), NavigationTypeOther), FrameLoader::callContinueLoadAfterNewWindowPolicy, request, 0, frameName, this);
}
