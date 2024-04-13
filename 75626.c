void ContainerNode::resumePostAttachCallbacks()
{
    if (s_attachDepth == 1) {
        RefPtr<ContainerNode> protect(this);

        if (s_postAttachCallbackQueue)
            dispatchPostAttachCallbacks();
        if (s_shouldReEnableMemoryCacheCallsAfterAttach) {
            s_shouldReEnableMemoryCacheCallsAfterAttach = false;
            if (Page* page = document()->page())
                page->setMemoryCacheClientCallsEnabled(true);
        }
        resourceLoadScheduler()->resumePendingRequests();
    }
    --s_attachDepth;
}
