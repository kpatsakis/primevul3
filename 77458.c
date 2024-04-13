Ewk_Hit_Test* ewk_frame_hit_test_new(const Evas_Object* ewkFrame, int x, int y)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, 0);

    WebCore::FrameView* view = smartData->frame->view();
    EINA_SAFETY_ON_NULL_RETURN_VAL(view, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame->contentRenderer(), 0);

    WebCore::HitTestResult result = smartData->frame->eventHandler()->hitTestResultAtPoint
                                        (view->windowToContents(WebCore::IntPoint(x, y)),
                                        /*allowShadowContent*/ false, /*ignoreClipping*/ true);

    if (result.scrollbar())
        return 0;
    if (!result.innerNode())
        return 0;

    Ewk_Hit_Test* hitTest = new Ewk_Hit_Test;
    hitTest->x = result.point().x();
    hitTest->y = result.point().y();
#if 0
    hitTest->bounding_box.x = result.boundingBox().x();
    hitTest->bounding_box.y = result.boundingBox().y();
    hitTest->bounding_box.width = result.boundingBox().width();
    hitTest->bounding_box.height = result.boundingBox().height();
#else
    hitTest->bounding_box.x = 0;
    hitTest->bounding_box.y = 0;
    hitTest->bounding_box.w = 0;
    hitTest->bounding_box.h = 0;
#endif

    WebCore::TextDirection direction;
    hitTest->title = eina_stringshare_add(result.title(direction).utf8().data());
    hitTest->alternate_text = eina_stringshare_add(result.altDisplayString().utf8().data());
    if (result.innerNonSharedNode() && result.innerNonSharedNode()->document()
        && result.innerNonSharedNode()->document()->frame())
        hitTest->frame = EWKPrivate::kitFrame(result.innerNonSharedNode()->document()->frame());

    hitTest->link.text = eina_stringshare_add(result.textContent().utf8().data());
    hitTest->link.url = eina_stringshare_add(result.absoluteLinkURL().string().utf8().data());
    hitTest->link.title = eina_stringshare_add(result.titleDisplayString().utf8().data());
    hitTest->link.target_frame = EWKPrivate::kitFrame(result.targetFrame());

    hitTest->image_uri = eina_stringshare_add(result.absoluteImageURL().string().utf8().data());
    hitTest->media_uri = eina_stringshare_add(result.absoluteMediaURL().string().utf8().data());

    int context = EWK_HIT_TEST_RESULT_CONTEXT_DOCUMENT;

    if (!result.absoluteLinkURL().isEmpty())
        context |= EWK_HIT_TEST_RESULT_CONTEXT_LINK;
    if (!result.absoluteImageURL().isEmpty())
        context |= EWK_HIT_TEST_RESULT_CONTEXT_IMAGE;
    if (!result.absoluteMediaURL().isEmpty())
        context |= EWK_HIT_TEST_RESULT_CONTEXT_MEDIA;
    if (result.isSelected())
        context |= EWK_HIT_TEST_RESULT_CONTEXT_SELECTION;
    if (result.isContentEditable())
        context |= EWK_HIT_TEST_RESULT_CONTEXT_EDITABLE;

    hitTest->context = static_cast<Ewk_Hit_Test_Result_Context>(context);

    return hitTest;
}
