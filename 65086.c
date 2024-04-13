FreeExpr(ExprDef *expr)
{
    if (!expr)
        return;

    switch (expr->expr.op) {
    case EXPR_ACTION_LIST:
    case EXPR_NEGATE:
    case EXPR_UNARY_PLUS:
    case EXPR_NOT:
    case EXPR_INVERT:
        FreeStmt((ParseCommon *) expr->unary.child);
        break;

    case EXPR_DIVIDE:
    case EXPR_ADD:
    case EXPR_SUBTRACT:
    case EXPR_MULTIPLY:
    case EXPR_ASSIGN:
        FreeStmt((ParseCommon *) expr->binary.left);
        FreeStmt((ParseCommon *) expr->binary.right);
        break;

    case EXPR_ACTION_DECL:
        FreeStmt((ParseCommon *) expr->action.args);
        break;

    case EXPR_ARRAY_REF:
        FreeStmt((ParseCommon *) expr->array_ref.entry);
        break;

    case EXPR_KEYSYM_LIST:
        darray_free(expr->keysym_list.syms);
        darray_free(expr->keysym_list.symsMapIndex);
        darray_free(expr->keysym_list.symsNumEntries);
        break;

    default:
        break;
    }
}
