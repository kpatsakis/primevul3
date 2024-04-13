void FrameLoader::closeAndRemoveChild(Frame* child)
{
    child->tree()->detachFromParent();

    child->setView(0);
    if (child->ownerElement() && child->page())
        child->page()->decrementSubframeCount();
    child->willDetachPage();
    child->detachFromPage();

    m_frame->tree()->removeChild(child);
}
