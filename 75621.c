bool ContainerNode::postAttachCallbacksAreSuspended()
{
    return s_attachDepth;
}
