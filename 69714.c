CJSON_PUBLIC(void) cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item)
{
    if ((object == NULL) || (string == NULL))
    {
        return;
    }

    add_item_to_object(object, string, create_reference(item, &global_hooks), &global_hooks, false);
}
