Eina_Bool ewk_frame_focused_element_geometry_get(const Evas_Object *ewkFrame, int *x, int *y, int *w, int *h)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    WebCore::Document* document = smartData->frame->document();
    if (!document)
        return false;
    WebCore::Node* focusedNode = document->focusedNode();
    if (!focusedNode)
        return false;
    WebCore::IntRect nodeRect = focusedNode->getPixelSnappedRect();
    if (x)
        *x = nodeRect.x();
    if (y)
        *y = nodeRect.y();
    if (w)
        *w = nodeRect.width();
    if (h)
        *h = nodeRect.height();
    return true;
}
