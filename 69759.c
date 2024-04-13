CJSON_PUBLIC(void) cJSON_ReplaceItemInArray(cJSON *array, int which, cJSON *newitem)
{
    if (which < 0)
    {
        return;
    }

    cJSON_ReplaceItemViaPointer(array, get_array_item(array, (size_t)which), newitem);
}
