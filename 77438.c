Eina_Iterator* ewk_frame_children_iterator_new(Evas_Object* ewkFrame)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    Eina_Iterator_Ewk_Frame* iterator = static_cast<Eina_Iterator_Ewk_Frame*>
                                  (calloc(1, sizeof(Eina_Iterator_Ewk_Frame)));
    if (!iterator)
        return 0;

    EINA_MAGIC_SET(&iterator->base, EINA_MAGIC_ITERATOR);
    iterator->base.next = FUNC_ITERATOR_NEXT(_ewk_frame_children_iterator_next);
    iterator->base.get_container = FUNC_ITERATOR_GET_CONTAINER(_ewk_frame_children_iterator_get_container);
    iterator->base.free = FUNC_ITERATOR_FREE(free);
    iterator->object = ewkFrame;
    iterator->currentIndex = 0;
    return &iterator->base;
}
