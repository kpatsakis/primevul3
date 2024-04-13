Eina_List* ewk_frame_resources_location_get(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->document(), 0);

    Eina_List* listOfImagesLocation = 0;

    RefPtr<WebCore::HTMLCollection> images = smartData->frame->document()->images();
    for (size_t index = 0; index < images->length(); ++index) {
        WebCore::HTMLImageElement* imageElement = static_cast<WebCore::HTMLImageElement*>(images->item(index));
        if (!imageElement || imageElement->src().isNull() || imageElement->src().isEmpty())
            continue;

        WTF::String imageLocation = WebCore::decodeURLEscapeSequences(imageElement->src().string());
        Eina_List* listIterator = 0;
        void* data = 0;
        Eina_Bool found = false;
        EINA_LIST_FOREACH(listOfImagesLocation, listIterator, data)
            if (found = !strcmp(static_cast<char*>(data), imageLocation.utf8().data()))
                break;
        if (found)
            continue;

        char* imageLocationCopy = strdup(imageLocation.utf8().data());
        if (!imageLocationCopy)
            goto out_of_memory_handler;
        listOfImagesLocation = eina_list_append(listOfImagesLocation, imageLocationCopy);
        if (eina_error_get())
            goto out_of_memory_handler;
    }
    return listOfImagesLocation;

out_of_memory_handler:
    CRITICAL("Could not allocate memory.");
    void* data;
    EINA_LIST_FREE(listOfImagesLocation, data)
        free(data);
    return 0;
}
