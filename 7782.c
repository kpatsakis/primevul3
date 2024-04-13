has_local_rule(const limited_rights_t *rights)
{
  return rights->access.sequence_number != NO_SEQUENCE_NUMBER;
}