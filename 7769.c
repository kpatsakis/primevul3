njs_module_add(njs_vm_t *vm, njs_str_t *name)
{
    njs_int_t           ret;
    njs_mod_t           *module;
    njs_lvlhsh_query_t  lhq;

    module = njs_mp_zalloc(vm->mem_pool, sizeof(njs_mod_t));
    if (njs_slow_path(module == NULL)) {
        njs_memory_error(vm);
        return NULL;
    }

    ret = njs_name_copy(vm, &module->name, name);
    if (njs_slow_path(ret != NJS_OK)) {
        njs_memory_error(vm);
        return NULL;
    }

    lhq.replace = 0;
    lhq.key = *name;
    lhq.key_hash = njs_djb_hash(name->start, name->length);
    lhq.value = module;
    lhq.pool = vm->mem_pool;
    lhq.proto = &njs_modules_hash_proto;

    ret = njs_lvlhsh_insert(&vm->shared->modules_hash, &lhq);
    if (njs_fast_path(ret == NJS_OK)) {
        return module;
    }

    njs_mp_free(vm->mem_pool, module->name.start);
    njs_mp_free(vm->mem_pool, module);

    njs_internal_error(vm, "lvlhsh insert failed");

    return NULL;
}