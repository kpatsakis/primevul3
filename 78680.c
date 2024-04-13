String FrameLoader::outgoingReferrer() const
{
    Frame* frame = m_frame;
    while (frame->document()->isSrcdocDocument()) {
        frame = frame->tree()->parent();
        ASSERT(frame);
    }
    return frame->loader()->m_outgoingReferrer;
}
