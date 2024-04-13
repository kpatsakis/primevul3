njs_module_hash_test(njs_lvlhsh_query_t *lhq, void *data)
{
    njs_mod_t  *module;

    module = data;

    if (njs_strstr_eq(&lhq->key, &module->name)) {
        return NJS_OK;
    }

    return NJS_DECLINED;
}