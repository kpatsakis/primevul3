static inline bool pageIsBeingDismissed(Document* document)
{
    Frame* frame = document->frame();
    return frame && frame->loader()->pageDismissalEventBeingDispatched() != FrameLoader::NoDismissal;
}
