njs_typed_array_generic_compare(const void *a, const void *b, void *c)
{
    double                      num;
    njs_int_t                   ret;
    njs_value_t                 arguments[3], retval;
    njs_typed_array_sort_ctx_t  *ctx;

    ctx = c;

    if (njs_slow_path(ctx->exception)) {
        return 0;
    }

    njs_set_undefined(&arguments[0]);
    njs_set_number(&arguments[1], ctx->get(a));
    njs_set_number(&arguments[2], ctx->get(b));

    ret = njs_function_apply(ctx->vm, ctx->function, arguments, 3, &retval);
    if (njs_slow_path(ret != NJS_OK)) {
        goto exception;
    }

    ret = njs_value_to_number(ctx->vm, &retval, &num);
    if (njs_slow_path(ret != NJS_OK)) {
        goto exception;
    }

    if (njs_slow_path(njs_is_detached_buffer(ctx->buffer))) {
        njs_type_error(ctx->vm, "detached buffer");
        goto exception;
    }

    if (njs_slow_path(isnan(num))) {
        return 0;
    }

    if (num != 0) {
        return (num > 0) - (num < 0);
    }

    return 0;

exception:

    ctx->exception = 1;

    return 0;
}