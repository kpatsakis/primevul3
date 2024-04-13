void ewk_frame_mixed_content_displayed_set(Evas_Object* ewkFrame, bool hasDisplayed)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    smartData->hasDisplayedMixedContent = hasDisplayed;

    if (hasDisplayed) {
        ewk_view_mixed_content_displayed_set(smartData->view, true);
        evas_object_smart_callback_call(ewkFrame, "mixedcontent,displayed", 0);
    }
}
