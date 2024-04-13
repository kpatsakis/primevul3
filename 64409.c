rule_criteria_require_rw(struct rule_criteria *criteria,
                         bool can_write_readonly)
{
    criteria->include_readonly = can_write_readonly;
}
