njs_typed_array_prototype_sort(njs_vm_t *vm, njs_value_t *args,
    njs_uint_t nargs, njs_index_t unused)
{
    u_char                      *base, *orig;
    int64_t                     length;
    uint32_t                    element_size;
    njs_value_t                 *this, *comparefn;
    njs_typed_array_t           *array;
    njs_array_buffer_t          *buffer;
    njs_typed_array_cmp_t       cmp;
    njs_typed_array_sort_ctx_t  ctx;

    this = njs_argument(args, 0);
    if (njs_slow_path(!njs_is_typed_array(this))) {
        njs_type_error(vm, "this is not a typed array");
        return NJS_ERROR;
    }

    array = njs_typed_array(this);
    if (njs_slow_path(njs_is_detached_buffer(array->buffer))) {
        njs_type_error(vm, "detached buffer");
        return NJS_ERROR;
    }

    ctx.vm = vm;
    ctx.buffer = array->buffer;
    ctx.exception = 0;

    comparefn = njs_arg(args, nargs, 1);

    if (njs_is_defined(comparefn)) {
        if (njs_slow_path(!njs_is_function(comparefn))) {
            njs_type_error(vm, "comparefn must be callable or undefined");
            return NJS_ERROR;
        }

        ctx.function = njs_function(comparefn);

    } else {
        ctx.function = NULL;
    }

    switch (array->type) {
    case NJS_OBJ_TYPE_UINT8_ARRAY:
    case NJS_OBJ_TYPE_UINT8_CLAMPED_ARRAY:
        cmp = njs_typed_array_compare_u8;
        ctx.get = njs_typed_array_get_u8;
        break;

    case NJS_OBJ_TYPE_INT8_ARRAY:
        cmp = njs_typed_array_compare_i8;
        ctx.get = njs_typed_array_get_i8;
        break;

    case NJS_OBJ_TYPE_UINT16_ARRAY:
        cmp = njs_typed_array_compare_u16;
        ctx.get = njs_typed_array_get_u16;
        break;

    case NJS_OBJ_TYPE_INT16_ARRAY:
        cmp = njs_typed_array_compare_i16;
        ctx.get = njs_typed_array_get_i16;
        break;

    case NJS_OBJ_TYPE_UINT32_ARRAY:
        cmp = njs_typed_array_compare_u32;
        ctx.get = njs_typed_array_get_u32;
        break;

    case NJS_OBJ_TYPE_INT32_ARRAY:
        cmp = njs_typed_array_compare_i32;
        ctx.get = njs_typed_array_get_i32;
        break;

    case NJS_OBJ_TYPE_FLOAT32_ARRAY:
        cmp = njs_typed_array_compare_f32;
        ctx.get = njs_typed_array_get_f32;
        break;

    default:

        /* NJS_OBJ_TYPE_FLOAT64_ARRAY. */

        cmp = njs_typed_array_compare_f64;
        ctx.get = njs_typed_array_get_f64;
        break;
    }

    buffer = njs_typed_array_writable(vm, array);
    if (njs_slow_path(buffer == NULL)) {
        return NJS_ERROR;
    }

    length = njs_typed_array_length(array);
    element_size = njs_typed_array_element_size(array->type);
    base = &buffer->u.u8[array->offset * element_size];
    orig = base;

    if (ctx.function != NULL) {
        cmp = njs_typed_array_generic_compare;
        base = njs_mp_alloc(vm->mem_pool, length * element_size);
        if (njs_slow_path(base == NULL)) {
            njs_memory_error(vm);
            return NJS_ERROR;
        }

        memcpy(base, &buffer->u.u8[array->offset * element_size],
               length * element_size);
    }

    njs_qsort(base, length, element_size, cmp, &ctx);
    if (ctx.function != NULL) {
        if (&buffer->u.u8[array->offset * element_size] == orig) {
            memcpy(orig, base, length * element_size);
        }

        njs_mp_free(vm->mem_pool, base);
    }

    if (njs_slow_path(ctx.exception)) {
        return NJS_ERROR;
    }

    njs_set_typed_array(&vm->retval, array);

    return NJS_OK;
}