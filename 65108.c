add_production(struct xkb_compose_table *table, struct scanner *s,
               const struct production *production)
{
    unsigned lhs_pos;
    uint32_t curr;
    struct compose_node *node;

    curr = 0;
    node = &darray_item(table->nodes, curr);

    /*
     * Insert the sequence to the trie, creating new nodes as needed.
     *
     * TODO: This can be sped up a bit by first trying the path that the
     * previous production took, and only then doing the linear search
     * through the trie levels.  This will work because sequences in the
     * Compose files are often clustered by a common prefix; especially
     * in the 1st and 2nd keysyms, which is where the largest variation
     * (thus, longest search) is.
     */
    for (lhs_pos = 0; lhs_pos < production->len; lhs_pos++) {
        while (production->lhs[lhs_pos] != node->keysym) {
            if (node->next == 0) {
                uint32_t next = add_node(table, production->lhs[lhs_pos]);
                /* Refetch since add_node could have realloc()ed. */
                node = &darray_item(table->nodes, curr);
                node->next = next;
            }

            curr = node->next;
            node = &darray_item(table->nodes, curr);
        }

        if (lhs_pos + 1 == production->len)
            break;

        if (node->is_leaf) {
            if (node->u.leaf.utf8 != 0 ||
                node->u.leaf.keysym != XKB_KEY_NoSymbol) {
                scanner_warn(s, "a sequence already exists which is a prefix of this sequence; overriding");
                node->u.leaf.utf8 = 0;
                node->u.leaf.keysym = XKB_KEY_NoSymbol;
            }

            {
                uint32_t successor = add_node(table, production->lhs[lhs_pos + 1]);
                /* Refetch since add_node could have realloc()ed. */
                node = &darray_item(table->nodes, curr);
                node->is_leaf = false;
                node->u.successor = successor;
            }
        }

        curr = node->u.successor;
        node = &darray_item(table->nodes, curr);
    }

    if (!node->is_leaf) {
        scanner_warn(s, "this compose sequence is a prefix of another; skipping line");
        return;
    }

    if (node->u.leaf.utf8 != 0 || node->u.leaf.keysym != XKB_KEY_NoSymbol) {
        bool same_string =
            (node->u.leaf.utf8 == 0 && !production->has_string) ||
            (
                node->u.leaf.utf8 != 0 && production->has_string &&
                streq(&darray_item(table->utf8, node->u.leaf.utf8),
                      production->string)
            );
        bool same_keysym =
            (node->u.leaf.keysym == XKB_KEY_NoSymbol && !production->has_keysym) ||
            (
                node->u.leaf.keysym != XKB_KEY_NoSymbol && production->has_keysym &&
                node->u.leaf.keysym == production->keysym
            );
        if (same_string && same_keysym) {
            scanner_warn(s, "this compose sequence is a duplicate of another; skipping line");
            return;
        }
        scanner_warn(s, "this compose sequence already exists; overriding");
    }

    if (production->has_string) {
        node->u.leaf.utf8 = darray_size(table->utf8);
        darray_append_items(table->utf8, production->string,
                            strlen(production->string) + 1);
    }
    if (production->has_keysym) {
        node->u.leaf.keysym = production->keysym;
    }
}
