static Eina_Bool _ewk_frame_children_iterator_next(Eina_Iterator_Ewk_Frame* iterator, Evas_Object** data)
{
    EWK_FRAME_SD_GET_OR_RETURN(iterator->object, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);

    WebCore::FrameTree* tree = smartData->frame->tree(); // check if it's still valid
    EINA_SAFETY_ON_NULL_RETURN_VAL(tree, false);

    if (iterator->currentIndex < tree->childCount()) {
        *data = EWKPrivate::kitFrame(tree->child(iterator->currentIndex++));
        return true;
    }

    return false;
}
