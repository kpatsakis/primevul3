rule_actions_destroy(const struct rule_actions *actions)
{
    if (actions) {
        ovsrcu_postpone(free, CONST_CAST(struct rule_actions *, actions));
    }
}
