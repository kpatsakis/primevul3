CJSON_PUBLIC(void) cJSON_free(void *object)
{
    global_hooks.deallocate(object);
}
