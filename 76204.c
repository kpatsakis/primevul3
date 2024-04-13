void ewk_view_popup_new(Evas_Object* ewkView, WebCore::PopupMenuClient* client, int selected, const WebCore::IntRect& rect)
{
    INF("ewkView=%p", ewkView);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv);

    if (priv->popup.menuClient)
        ewk_view_popup_destroy(ewkView);

    priv->popup.menuClient = client;

    const int size = client->listSize();
    for (int i = 0; i < size; ++i) {
        Ewk_Menu_Item* item = static_cast<Ewk_Menu_Item*>(malloc(sizeof(*item)));
        if (client->itemIsSeparator(i))
            item->type = EWK_MENU_SEPARATOR;
        else if (client->itemIsLabel(i))
            item->type = EWK_MENU_GROUP;
        else
            item->type = EWK_MENU_OPTION;
        item->text = eina_stringshare_add(client->itemText(i).utf8().data());

        priv->popup.menu.items = eina_list_append(priv->popup.menu.items, item);
    }

    priv->popup.menu.x = rect.x();
    priv->popup.menu.y = rect.y();
    priv->popup.menu.width = rect.width();
    priv->popup.menu.height = rect.height();
    evas_object_smart_callback_call(ewkView, "popup,create", &priv->popup.menu);
}
