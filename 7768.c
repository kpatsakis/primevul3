njs_module_find(njs_vm_t *vm, njs_str_t *name, njs_bool_t shared)
{
    njs_int_t           ret;
    njs_mod_t           *shrd, *module;
    njs_object_t        *object;
    njs_lvlhsh_query_t  lhq;

    lhq.key = *name;
    lhq.key_hash = njs_djb_hash(name->start, name->length);
    lhq.proto = &njs_modules_hash_proto;

    if (njs_lvlhsh_find(&vm->modules_hash, &lhq) == NJS_OK) {
        return lhq.value;
    }

    if (njs_lvlhsh_find(&vm->shared->modules_hash, &lhq) == NJS_OK) {
        shrd = lhq.value;

        if (shared) {
            return shrd;
        }

        module = njs_mp_alloc(vm->mem_pool, sizeof(njs_mod_t));
        if (njs_slow_path(module == NULL)) {
            njs_memory_error(vm);
            return NULL;
        }

        memcpy(module, shrd, sizeof(njs_mod_t));

        object = njs_object_value_copy(vm, &module->value);
        if (njs_slow_path(object == NULL)) {
            return NULL;
        }

        lhq.replace = 0;
        lhq.value = module;
        lhq.pool = vm->mem_pool;

        ret = njs_lvlhsh_insert(&vm->modules_hash, &lhq);
        if (njs_fast_path(ret == NJS_OK)) {
            return module;
        }
    }

    return NULL;
}