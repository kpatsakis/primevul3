rule_is_readonly(const struct rule *rule)
{
    const struct oftable *table = &rule->ofproto->tables[rule->table_id];
    return (table->flags & OFTABLE_READONLY) != 0;
}
