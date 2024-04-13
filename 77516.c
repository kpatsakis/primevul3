void ewk_frame_view_set(Evas_Object* ewkFrame, Evas_Object* newParent)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);

    evas_object_smart_member_del(ewkFrame);
    evas_object_smart_member_add(ewkFrame, newParent);

    smartData->view = newParent;
}
