next_learn_with_delete(const struct rule_actions *actions,
                       const struct ofpact_learn *start)
{
    const struct ofpact *pos;

    for (pos = start ? ofpact_next(&start->ofpact) : actions->ofpacts;
         pos < ofpact_end(actions->ofpacts, actions->ofpacts_len);
         pos = ofpact_next(pos)) {
        if (pos->type == OFPACT_LEARN) {
            const struct ofpact_learn *learn = ofpact_get_LEARN(pos);
            if (learn->flags & NX_LEARN_F_DELETE_LEARNED) {
                return learn;
            }
        }
    }

    return NULL;
}
