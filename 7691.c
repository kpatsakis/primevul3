njs_data_view_constructor(njs_vm_t *vm, njs_value_t *args, njs_uint_t nargs,
    njs_index_t unused)
{
    uint64_t            size, offset;
    njs_int_t           ret;
    njs_data_view_t     *view;
    njs_array_buffer_t  *buffer;

    if (!vm->top_frame->ctor) {
        njs_type_error(vm, "Constructor of DataView requires 'new'");
        return NJS_ERROR;
    }

    if (!njs_is_array_buffer(njs_arg(args, nargs, 1))) {
        njs_type_error(vm, "buffer is not an ArrayBuffer");
        return NJS_ERROR;
    }

    size = 0;
    offset = 0;

    ret = njs_value_to_index(vm, njs_arg(args, nargs, 2), &offset);
    if (njs_slow_path(ret != NJS_OK)) {
        return NJS_ERROR;
    }

    buffer = njs_array_buffer(njs_argument(args, 1));
    if (njs_slow_path(njs_is_detached_buffer(buffer))) {
        njs_type_error(vm, "detached buffer");
        return NJS_ERROR;
    }

    if (!njs_is_undefined(njs_arg(args, nargs, 3))) {
        ret = njs_value_to_index(vm, njs_argument(args, 3), &size);
        if (njs_slow_path(ret != NJS_OK)) {
            return NJS_ERROR;
        }

        if (njs_slow_path((offset + size) > buffer->size)) {
            njs_range_error(vm, "Invalid DataView length: %uL", size);
            return NJS_ERROR;
        }

    } else {
        if (offset > buffer->size) {
            njs_range_error(vm, "byteOffset %uL is outside the bound of "
                            "the buffer", offset);
            return NJS_ERROR;
        }

        size = buffer->size - offset;
    }

    view = njs_mp_zalloc(vm->mem_pool, sizeof(njs_data_view_t));
    if (njs_slow_path(view == NULL)) {
        goto memory_error;
    }

    view->buffer = buffer;
    view->offset = offset;
    view->byte_length = size;
    view->type = NJS_OBJ_TYPE_DATA_VIEW;

    njs_lvlhsh_init(&view->object.hash);
    njs_lvlhsh_init(&view->object.shared_hash);
    view->object.__proto__ = &vm->prototypes[view->type].object;
    view->object.type = NJS_DATA_VIEW;
    view->object.extensible = 1;

    njs_set_data_view(&vm->retval, view);

    return NJS_OK;

memory_error:

    njs_memory_error(vm);

    return NJS_ERROR;
}