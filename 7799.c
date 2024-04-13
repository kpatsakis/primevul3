combine_access(limited_rights_t *target,
               const limited_rights_t *rights)
{
  /* This implies the check for NO_SEQUENCE_NUMBER, i.e no rights being
   * specified. */
  if (target->access.sequence_number < rights->access.sequence_number)
    target->access = rights->access;
}