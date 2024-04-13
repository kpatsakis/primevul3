void ContainerNode::scheduleSetNeedsStyleRecalc(StyleChangeType changeType)
{
    if (postAttachCallbacksAreSuspended())
        queuePostAttachCallback(needsStyleRecalcCallback, this, static_cast<unsigned>(changeType));
    else
        setNeedsStyleRecalc(changeType);
}
