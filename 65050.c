ExprResolveString(struct xkb_context *ctx, const ExprDef *expr,
                  xkb_atom_t *val_rtrn)
{
    switch (expr->expr.op) {
    case EXPR_VALUE:
        if (expr->expr.value_type != EXPR_TYPE_STRING) {
            log_err(ctx, "Found constant of type %s, expected a string\n",
                    expr_value_type_to_string(expr->expr.value_type));
            return false;
        }

        *val_rtrn = expr->string.str;
        return true;

    case EXPR_IDENT:
        log_err(ctx, "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, expr->ident.ident));
        return false;

    case EXPR_FIELD_REF:
        log_err(ctx, "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, expr->field_ref.element),
                xkb_atom_text(ctx, expr->field_ref.field));
        return false;

    case EXPR_ADD:
    case EXPR_SUBTRACT:
    case EXPR_MULTIPLY:
    case EXPR_DIVIDE:
    case EXPR_ASSIGN:
    case EXPR_NEGATE:
    case EXPR_INVERT:
    case EXPR_NOT:
    case EXPR_UNARY_PLUS:
        log_err(ctx, "%s of strings not permitted\n",
                expr_op_type_to_string(expr->expr.op));
        return false;

    default:
        log_wsgo(ctx, "Unknown operator %d in ResolveString\n",
                 expr->expr.op);
        break;
    }
    return false;
}
