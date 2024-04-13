void RenderLayerCompositor::attachRootLayer(RootLayerAttachment attachment)
{
    if (!m_rootContentLayer)
        return;

    switch (attachment) {
        case RootLayerUnattached:
            ASSERT_NOT_REACHED();
            break;
        case RootLayerAttachedViaChromeClient: {
            Frame& frame = m_renderView->frameView()->frame();
            Page* page = frame.page();
            if (!page)
                return;
            page->chrome().client().attachRootGraphicsLayer(&frame, rootGraphicsLayer());
            break;
        }
        case RootLayerAttachedViaEnclosingFrame: {
            HTMLFrameOwnerElement* ownerElement = m_renderView->document().ownerElement();
            ASSERT(ownerElement);
            DeprecatedScheduleStyleRecalcDuringCompositingUpdate marker(ownerElement->document().lifecycle());
            ownerElement->scheduleLayerUpdate();
            break;
        }
    }

    m_rootLayerAttachment = attachment;
}
