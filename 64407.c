rule_criteria_destroy(struct rule_criteria *criteria)
{
    cls_rule_destroy(&criteria->cr);
    criteria->version = OVS_VERSION_NOT_REMOVED; /* Mark as destroyed. */
}
