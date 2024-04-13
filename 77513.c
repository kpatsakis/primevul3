Eina_Bool ewk_frame_uri_set(Evas_Object* ewkFrame, const char* uri)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    WebCore::KURL kurl(WebCore::KURL(), WTF::String::fromUTF8(uri));
    WebCore::ResourceRequest req(kurl);
    WebCore::FrameLoader* loader = smartData->frame->loader();
    loader->load(req, false);
    return true;
}
