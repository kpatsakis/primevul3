get_conjunctions(const struct ofputil_flow_mod *fm,
                 struct cls_conjunction **conjsp, size_t *n_conjsp)
{
    struct cls_conjunction *conjs = NULL;
    int n_conjs = 0;

    const struct ofpact *ofpact;
    OFPACT_FOR_EACH (ofpact, fm->ofpacts, fm->ofpacts_len) {
        if (ofpact->type == OFPACT_CONJUNCTION) {
            n_conjs++;
        } else if (ofpact->type != OFPACT_NOTE) {
            /* "conjunction" may appear with "note" actions but not with any
             * other type of actions. */
            ovs_assert(!n_conjs);
            break;
        }
    }
    if (n_conjs) {
        int i = 0;

        conjs = xzalloc(n_conjs * sizeof *conjs);
        OFPACT_FOR_EACH (ofpact, fm->ofpacts, fm->ofpacts_len) {
            if (ofpact->type == OFPACT_CONJUNCTION) {
                struct ofpact_conjunction *oc = ofpact_get_CONJUNCTION(ofpact);
                conjs[i].clause = oc->clause;
                conjs[i].n_clauses = oc->n_clauses;
                conjs[i].id = oc->id;
                i++;
            }
        }
    }

    *conjsp = conjs;
    *n_conjsp = n_conjs;
}
