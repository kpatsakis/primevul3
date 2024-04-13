static void cs_mark(const char *cs_name, int subr)
{
    byte *data;
    int i, b, cs_len;
    int last_cmd = 0;
    integer a, a1, a2;
    unsigned short cr;
    static integer lastargOtherSubr3 = 3;       /* the argument of last call to
                                                   OtherSubrs[3] */
    cs_entry *ptr;
    cc_entry *cc;
    if (cs_name == NULL) {
        check_subr(subr);
        ptr = subr_tab + subr;
        if (!ptr->valid)
            return;
    } else {
        if (cs_notdef != NULL &&
            (cs_name == notdef || strcmp(cs_name, notdef) == 0))
            ptr = cs_notdef;
        else {
            for (ptr = cs_tab; ptr < cs_ptr; ptr++)
                if (strcmp(ptr->name, cs_name) == 0)
                    break;
            if (ptr == cs_ptr) {
                pdftex_warn("glyph `%s' undefined", cs_name);
                return;
            }
            if (ptr->name == notdef)
                cs_notdef = ptr;
        }
    }
    /* only marked CharString entries and invalid entries can be skipped;
       valid marked subrs must be parsed to keep the stack in sync */
    if (!ptr->valid || (ptr->used && cs_name != NULL))
        return;
    ptr->used = true;
    cr = 4330;
    cs_len = ptr->cslen;
    data = ptr->data + 4;
    for (i = 0; i < t1_lenIV; i++, cs_len--)
        cs_getchar();
    while (cs_len > 0) {
        --cs_len;
        b = cs_getchar();
        if (b >= 32) {
            if (b <= 246)
                a = b - 139;
            else if (b <= 250) {
                --cs_len;
                a = ((b - 247) << 8) + 108 + cs_getchar();
            } else if (b <= 254) {
                --cs_len;
                a = -((b - 251) << 8) - 108 - cs_getchar();
            } else {
                cs_len -= 4;
                a = (cs_getchar() & 0xff) << 24;
                a |= (cs_getchar() & 0xff) << 16;
                a |= (cs_getchar() & 0xff) << 8;
                a |= (cs_getchar() & 0xff) << 0;
                if (sizeof(integer) > 4 && (a & 0x80000000))
                    a |= ~0x7FFFFFFF;
            }
            cc_push(a);
        } else {
            if (b == CS_ESCAPE) {
                b = cs_getchar() + CS_1BYTE_MAX;
                cs_len--;
            }
            if (b >= CS_MAX) {
                cs_fail(cs_name, subr, "command value out of range: %i",
                        (int) b);
                goto cs_error;
            }
            cc = cc_tab + b;
            if (!cc->valid) {
                cs_fail(cs_name, subr, "command not valid: %i", (int) b);
                goto cs_error;
            }
            if (cc->bottom) {
                if (stack_ptr - cc_stack < cc->nargs)
                    cs_fail(cs_name, subr,
                            "less arguments on stack (%i) than required (%i)",
                            (int) (stack_ptr - cc_stack), (int) cc->nargs);
                else if (stack_ptr - cc_stack > cc->nargs)
                    cs_fail(cs_name, subr,
                            "more arguments on stack (%i) than required (%i)",
                            (int) (stack_ptr - cc_stack), (int) cc->nargs);
            }
            last_cmd = b;
            switch (cc - cc_tab) {
            case CS_CALLSUBR:
                a1 = cc_get(-1);
                cc_pop(1);
                mark_subr(a1);
                if (!subr_tab[a1].valid) {
                    cs_fail(cs_name, subr, "cannot call subr (%i)", (int) a1);
                    goto cs_error;
                }
                break;
            case CS_DIV:
                cc_pop(2);
                cc_push(0);
                break;
            case CS_CALLOTHERSUBR:
                if (cc_get(-1) == 3)
                    lastargOtherSubr3 = cc_get(-3);
                a1 = cc_get(-2) + 2;
                cc_pop(a1);
                break;
            case CS_POP:
                cc_push(lastargOtherSubr3);
                /* the only case when we care about the value being pushed onto
                   stack is when POP follows CALLOTHERSUBR (changing hints by
                   OtherSubrs[3])
                 */
                break;
            case CS_SEAC:
                a1 = cc_get(3);
                a2 = cc_get(4);
                cc_clear();
                mark_cs(standard_glyph_names[a1]);
                mark_cs(standard_glyph_names[a2]);
                break;
            default:
                if (cc->clear)
                    cc_clear();
            }
        }
    }
    if (cs_name == NULL && last_cmd != CS_RETURN) {
        pdftex_warn("last command in subr `%i' is not a RETURN; "
                    "I will add it now but please consider fixing the font",
                    (int) subr);
        append_cs_return(ptr);
    }
    return;
  cs_error:                    /* an error occured during parsing */
    cc_clear();
    ptr->valid = false;
    ptr->used = false;
}
