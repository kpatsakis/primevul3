ExprResolveKeyCode(struct xkb_context *ctx, const ExprDef *expr,
                   xkb_keycode_t *kc)
{
    xkb_keycode_t leftRtrn, rightRtrn;

    switch (expr->expr.op) {
    case EXPR_VALUE:
        if (expr->expr.value_type != EXPR_TYPE_INT) {
            log_err(ctx,
                    "Found constant of type %s where an int was expected\n",
                    expr_value_type_to_string(expr->expr.value_type));
            return false;
        }

        *kc = (xkb_keycode_t) expr->integer.ival;
        return true;

    case EXPR_ADD:
    case EXPR_SUBTRACT:
    case EXPR_MULTIPLY:
    case EXPR_DIVIDE:
        if (!ExprResolveKeyCode(ctx, expr->binary.left, &leftRtrn) ||
            !ExprResolveKeyCode(ctx, expr->binary.right, &rightRtrn))
            return false;

        switch (expr->expr.op) {
        case EXPR_ADD:
            *kc = leftRtrn + rightRtrn;
            break;
        case EXPR_SUBTRACT:
            *kc = leftRtrn - rightRtrn;
            break;
        case EXPR_MULTIPLY:
            *kc = leftRtrn * rightRtrn;
            break;
        case EXPR_DIVIDE:
            if (rightRtrn == 0) {
                log_err(ctx, "Cannot divide by zero: %d / %d\n",
                        leftRtrn, rightRtrn);
                return false;
            }

            *kc = leftRtrn / rightRtrn;
            break;
        default:
            break;
        }

        return true;

    case EXPR_NEGATE:
        if (!ExprResolveKeyCode(ctx, expr->unary.child, &leftRtrn))
            return false;

        *kc = ~leftRtrn;
        return true;

    case EXPR_UNARY_PLUS:
        return ExprResolveKeyCode(ctx, expr->unary.child, kc);

    default:
        log_wsgo(ctx, "Unknown operator %d in ResolveKeyCode\n",
                 expr->expr.op);
        break;
    }

    return false;
}
