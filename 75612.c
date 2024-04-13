void ContainerNode::dispatchPostAttachCallbacks()
{
    for (size_t i = 0; i < s_postAttachCallbackQueue->size(); ++i) {
        const CallbackInfo& info = (*s_postAttachCallbackQueue)[i];
        NodeCallback callback = info.first;
        CallbackParameters params = info.second;

        callback(params.first.get(), params.second);
    }
    s_postAttachCallbackQueue->clear();
}
