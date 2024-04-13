njs_module_lookup(njs_vm_t *vm, const njs_str_t *cwd, njs_module_info_t *info)
{
    njs_int_t   ret;
    njs_str_t   *path;
    njs_uint_t  i;

    if (info->name.start[0] == '/') {
        return njs_module_path(vm, NULL, info);
    }

    ret = njs_module_path(vm, cwd, info);

    if (ret != NJS_DECLINED) {
        return ret;
    }

    if (vm->paths == NULL) {
        return NJS_DECLINED;
    }

    path = vm->paths->start;

    for (i = 0; i < vm->paths->items; i++) {
        ret = njs_module_path(vm, path, info);

        if (ret != NJS_DECLINED) {
            return ret;
        }

        path++;
    }

    return NJS_DECLINED;
}