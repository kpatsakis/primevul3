static void add_kid(int id, xref_t *xref)
{
    /* Make some space */
    if (((xref->n_kids + 1) * KID_SIZE) > (xref->n_kids_allocs*KIDS_PER_ALLOC))
      xref->kids = realloc(
          xref->kids, (++xref->n_kids_allocs)*(KIDS_PER_ALLOC * KID_SIZE));

    xref->kids[xref->n_kids++] = id;
}
