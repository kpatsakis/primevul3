void Element::updateFocusAppearance(bool /*restorePreviousSelection*/)
{
    if (isRootEditableElement()) {
        Frame* frame = document()->frame();
        if (!frame)
            return;
        
        if (this == frame->selection()->rootEditableElement())
            return;

        VisibleSelection newSelection = VisibleSelection(firstPositionInOrBeforeNode(this), DOWNSTREAM);
        
        if (frame->selection()->shouldChangeSelection(newSelection)) {
            frame->selection()->setSelection(newSelection);
            frame->selection()->revealSelection();
        }
    } else if (renderer() && !renderer()->isWidget())
        renderer()->scrollRectToVisible(boundingBox());
}
