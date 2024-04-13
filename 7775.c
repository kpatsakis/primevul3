njs_default_module_loader(njs_vm_t *vm, njs_external_ptr_t external,
    njs_str_t *name)
{
    njs_int_t          ret;
    njs_str_t          cwd, text;
    njs_parser_t       *prev;
    njs_mod_t          *module;
    njs_module_info_t  info;

    prev = external;

    njs_memzero(&info, sizeof(njs_module_info_t));

    info.name = *name;
    njs_file_dirname(&prev->lexer->file, &cwd);

    ret = njs_module_lookup(vm, &cwd, &info);
    if (njs_slow_path(ret != NJS_OK)) {
        return NULL;
    }

    ret = njs_module_read(vm, info.fd, &text);

    (void) close(info.fd);

    if (njs_slow_path(ret != NJS_OK)) {
        njs_internal_error(vm, "while reading \"%V\" module", &info.file);
        return NULL;
    }

    module = njs_vm_compile_module(vm, &info.file, &text.start,
                                   &text.start[text.length]);

    njs_mp_free(vm->mem_pool, text.start);

    return module;
}