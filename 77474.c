const char* ewk_frame_name_get(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);

    if (!smartData->frame) {
        ERR("could not get name of uninitialized frame.");
        return 0;
    }

    const WTF::String frameName = smartData->frame->tree()->uniqueName();

    if ((smartData->name) && (smartData->name == frameName))
        return smartData->name;

    eina_stringshare_replace_length(&(smartData->name), frameName.utf8().data(), frameName.length());

    return smartData->name;
}
