void RenderLayerCompositor::notifyIFramesOfCompositingChange()
{
    if (!m_renderView->frameView())
        return;
    Frame& frame = m_renderView->frameView()->frame();

    for (Frame* child = frame.tree().firstChild(); child; child = child->tree().traverseNext(&frame)) {
        if (!child->document())
            continue; // FIXME: Can this happen?
        if (HTMLFrameOwnerElement* ownerElement = child->document()->ownerElement()) {
            DeprecatedScheduleStyleRecalcDuringCompositingUpdate marker(ownerElement->document().lifecycle());
            ownerElement->scheduleLayerUpdate();
        }
    }

    if (HTMLFrameOwnerElement* ownerElement = m_renderView->document().ownerElement()) {
        ownerElement->document().renderView()->compositor()->setNeedsToRecomputeCompositingRequirements();
        DeprecatedScheduleStyleRecalcDuringCompositingUpdate marker(ownerElement->document().lifecycle());
        ownerElement->scheduleLayerUpdate();
    }
}
