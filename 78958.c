void Element::scheduleSyntheticStyleChange()
{
    if (postAttachCallbacksAreSuspended())
        queuePostAttachCallback(needsSyntheticStyleChangeCallback, this);
    else
        setNeedsStyleRecalc(SyntheticStyleChange);
}
