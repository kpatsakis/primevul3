Eina_Bool ewk_view_context_menu_forward_event(Evas_Object* ewkView, const Evas_Event_Mouse_Down* downEvent)
{
#if ENABLE(CONTEXT_MENUS)
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    Eina_Bool mouse_press_handled = false;

    priv->page->contextMenuController()->clearContextMenu();
    WebCore::Frame* mainFrame = priv->page->mainFrame();
    Evas_Coord x, y;
    evas_object_geometry_get(smartData->self, &x, &y, 0, 0);

    WebCore::PlatformMouseEvent event(downEvent, WebCore::IntPoint(x, y));

    if (mainFrame->view()) {
        mouse_press_handled =
            mainFrame->eventHandler()->handleMousePressEvent(event);
    }

    if (mainFrame->eventHandler()->sendContextMenuEvent(event))
        return false;

    WebCore::ContextMenu* coreMenu =
        priv->page->contextMenuController()->contextMenu();
    if (!coreMenu) {
        return mouse_press_handled;
    }

    return true;
#else
    return false;
#endif
}
