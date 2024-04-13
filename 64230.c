group_add_rule(struct ofgroup *group, struct rule *rule)
{
    rule_collection_add(&group->rules, rule);
}
