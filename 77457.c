void ewk_frame_hit_test_free(Ewk_Hit_Test* hitTest)
{
    EINA_SAFETY_ON_NULL_RETURN(hitTest);
    eina_stringshare_del(hitTest->title);
    eina_stringshare_del(hitTest->alternate_text);
    eina_stringshare_del(hitTest->link.text);
    eina_stringshare_del(hitTest->link.url);
    eina_stringshare_del(hitTest->link.title);
    eina_stringshare_del(hitTest->image_uri);
    eina_stringshare_del(hitTest->media_uri);
    delete hitTest;
}
