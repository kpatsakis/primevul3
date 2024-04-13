static bool shouldOpenInNewWindow(Frame* targetFrame, const FrameLoadRequest& request, const NavigationAction& action)
{
    if (!targetFrame && !request.frameName().isEmpty())
        return true;
    if (!request.formState())
        return false;
    NavigationPolicy navigationPolicy = NavigationPolicyCurrentTab;
    if (!action.specifiesNavigationPolicy(&navigationPolicy))
        return false;
    return navigationPolicy != NavigationPolicyCurrentTab;
}
