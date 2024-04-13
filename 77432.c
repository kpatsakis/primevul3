WebCore::Frame* coreFrame(const Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    return smartData->frame;
}
