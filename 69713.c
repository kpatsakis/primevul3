CJSON_PUBLIC(void) cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item)
{
    if (array == NULL)
    {
        return;
    }

    add_item_to_array(array, create_reference(item, &global_hooks));
}
