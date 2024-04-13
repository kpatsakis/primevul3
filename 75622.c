void ContainerNode::queuePostAttachCallback(NodeCallback callback, Node* node, unsigned callbackData)
{
    if (!s_postAttachCallbackQueue)
        s_postAttachCallbackQueue = new NodeCallbackQueue;
    
    s_postAttachCallbackQueue->append(CallbackInfo(callback, CallbackParameters(node, callbackData)));
}
