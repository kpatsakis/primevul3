combine_right_limits(limited_rights_t *target,
                     const limited_rights_t *rights)
{
  target->max_rights |= rights->max_rights;
  target->min_rights &= rights->min_rights;
}