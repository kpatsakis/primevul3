bool ewk_frame_child_add(Evas_Object* ewkFrame, WTF::PassRefPtr<WebCore::Frame> child, const WTF::String& name, const WebCore::KURL& url, const WTF::String& referrer)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    char buffer[256];
    Evas_Object* frame;
    WebCore::Frame* coreFrame;

    frame = ewk_frame_add(smartData->base.evas);
    if (!frame) {
        ERR("Could not create ewk_frame object.");
        return false;
    }

    coreFrame = child.get();
    if (coreFrame->tree())
        coreFrame->tree()->setName(name);
    else
        ERR("no tree for child object");
    smartData->frame->tree()->appendChild(child);

    if (!ewk_frame_init(frame, smartData->view, coreFrame)) {
        evas_object_del(frame);
        return false;
    }
    snprintf(buffer, sizeof(buffer), "EWK_Frame:child/%s", name.utf8().data());
    evas_object_name_set(frame, buffer);
    evas_object_smart_member_add(frame, ewkFrame);
    evas_object_show(frame);

    if (!coreFrame->page()) {
        evas_object_del(frame);
        return true;
    }

    evas_object_smart_callback_call(smartData->view, "frame,created", frame);
    smartData->frame->loader()->loadURLIntoChildFrame(url, referrer, coreFrame);

    if (!coreFrame->tree()->parent()) {
        evas_object_del(frame);
        return true;
    }

    return true;
}
