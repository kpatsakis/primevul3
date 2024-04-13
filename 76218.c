bool ewk_view_run_open_panel(Evas_Object* ewkView, Evas_Object* frame, bool allowsMultipleFiles, const WTF::Vector<WTF::String>& acceptMIMETypes, Eina_List** selectedFilenames)
{
    DBG("ewkView=%p frame=%p allows_multiple_files=%d", ewkView, frame, allowsMultipleFiles);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api, false);

    if (!smartData->api->run_open_panel)
        return false;

    *selectedFilenames = 0;

    Eina_List* cAcceptMIMETypes = 0;
    for (WTF::Vector<WTF::String>::const_iterator iterator = acceptMIMETypes.begin(); iterator != acceptMIMETypes.end(); ++iterator)
        cAcceptMIMETypes = eina_list_append(cAcceptMIMETypes, strdup((*iterator).utf8().data()));

    bool confirm = smartData->api->run_open_panel(smartData, frame, allowsMultipleFiles, cAcceptMIMETypes, selectedFilenames);
    if (!confirm && *selectedFilenames)
        ERR("Canceled file selection, but selected filenames != 0. Free names before return.");

    void* item = 0;
    EINA_LIST_FREE(cAcceptMIMETypes, item)
        free(item);

    return confirm;
}
