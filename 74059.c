static void list_data_recursivematch(struct listargs *listargs) {
    struct list_rock_recursivematch rock;

    rock.count = 0;
    rock.listargs = listargs;
    construct_hash_table(&rock.table, 100, 1);

    /* find */
    mboxlist_findsubmulti(&imapd_namespace, &listargs->pat, imapd_userisadmin, imapd_userid,
                          imapd_authstate, recursivematch_cb, &rock, 1);

    if (rock.count) {
        int i;
        int entries = rock.count;

        /* sort */
        rock.array = xmalloc(entries * (sizeof(struct list_entry)));
        hash_enumerate(&rock.table, copy_to_array, &rock);
        qsort(rock.array, entries, sizeof(struct list_entry),
              list_entry_comparator);
        assert(rock.count == 0);

        /* print */
        for (i = 0; i < entries; i++) {
            if (!rock.array[i].name) continue;
            mbentry_t *mbentry = NULL;
            mboxlist_lookup(rock.array[i].name, &mbentry, NULL);
            list_response(rock.array[i].name,
                          mbentry,
                          rock.array[i].attributes,
                          rock.listargs);
            mboxlist_entry_free(&mbentry);
        }

        free(rock.array);
    }

    free_hash_table(&rock.table, free);
}
