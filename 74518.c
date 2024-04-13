static inline bool canReferToParentFrameEncoding(const Frame* frame, const Frame* parentFrame) 
{
    return parentFrame && parentFrame->document()->securityOrigin()->canAccess(frame->document()->securityOrigin());
}
