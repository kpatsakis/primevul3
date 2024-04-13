ExprResolveIntegerLookup(struct xkb_context *ctx, const ExprDef *expr,
                         int *val_rtrn, IdentLookupFunc lookup,
                         const void *lookupPriv)
{
    bool ok = false;
    int l, r;
    unsigned u;
    ExprDef *left, *right;

    switch (expr->expr.op) {
    case EXPR_VALUE:
        if (expr->expr.value_type != EXPR_TYPE_INT) {
            log_err(ctx,
                    "Found constant of type %s where an int was expected\n",
                    expr_value_type_to_string(expr->expr.value_type));
            return false;
        }

        *val_rtrn = expr->integer.ival;
        return true;

    case EXPR_IDENT:
        if (lookup)
            ok = lookup(ctx, lookupPriv, expr->ident.ident, EXPR_TYPE_INT, &u);

        if (!ok)
            log_err(ctx, "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, expr->ident.ident));
        else
            *val_rtrn = (int) u;

        return ok;

    case EXPR_FIELD_REF:
        log_err(ctx, "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, expr->field_ref.element),
                xkb_atom_text(ctx, expr->field_ref.field));
        return false;

    case EXPR_ADD:
    case EXPR_SUBTRACT:
    case EXPR_MULTIPLY:
    case EXPR_DIVIDE:
        left = expr->binary.left;
        right = expr->binary.right;
        if (!ExprResolveIntegerLookup(ctx, left, &l, lookup, lookupPriv) ||
            !ExprResolveIntegerLookup(ctx, right, &r, lookup, lookupPriv))
            return false;

        switch (expr->expr.op) {
        case EXPR_ADD:
            *val_rtrn = l + r;
            break;
        case EXPR_SUBTRACT:
            *val_rtrn = l - r;
            break;
        case EXPR_MULTIPLY:
            *val_rtrn = l * r;
            break;
        case EXPR_DIVIDE:
            if (r == 0) {
                log_err(ctx, "Cannot divide by zero: %d / %d\n", l, r);
                return false;
            }
            *val_rtrn = l / r;
            break;
        default:
            log_err(ctx, "%s of integers not permitted\n",
                    expr_op_type_to_string(expr->expr.op));
            return false;
        }

        return true;

    case EXPR_ASSIGN:
        log_wsgo(ctx, "Assignment operator not implemented yet\n");
        break;

    case EXPR_NOT:
        log_err(ctx, "The ! operator cannot be applied to an integer\n");
        return false;

    case EXPR_INVERT:
    case EXPR_NEGATE:
        left = expr->unary.child;
        if (!ExprResolveIntegerLookup(ctx, left, &l, lookup, lookupPriv))
            return false;

        *val_rtrn = (expr->expr.op == EXPR_NEGATE ? -l : ~l);
        return true;

    case EXPR_UNARY_PLUS:
        left = expr->unary.child;
        return ExprResolveIntegerLookup(ctx, left, val_rtrn, lookup,
                                        lookupPriv);

    default:
        log_wsgo(ctx, "Unknown operator %d in ResolveInteger\n",
                 expr->expr.op);
        break;
    }

    return false;
}
