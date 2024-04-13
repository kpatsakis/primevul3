void FrameLoader::continueLoadAfterNewWindowPolicy(const ResourceRequest& request,
    PassRefPtr<FormState> formState, const String& frameName, bool shouldContinue)
{
    if (!shouldContinue)
        return;

    RefPtr<Frame> frame = m_frame;
    RefPtr<Frame> mainFrame = m_client->dispatchCreatePage();
    if (!mainFrame)
        return;

    if (frameName != "_blank")
        mainFrame->tree()->setName(frameName);

    mainFrame->page()->setOpenedByDOM();
    mainFrame->loader()->m_client->dispatchShow();
    if (!m_suppressOpenerInNewFrame)
        mainFrame->loader()->setOpener(frame.get());
    mainFrame->loader()->loadWithNavigationAction(request, NavigationAction(), false, FrameLoadTypeStandard, formState);
}
