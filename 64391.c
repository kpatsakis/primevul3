oftable_vacancy(const struct oftable *t)
{
    return (!t->max_flows ? 100
            : t->n_flows >= t->max_flows ? 0
            : (t->max_flows - t->n_flows) * 100.0 / t->max_flows);
}
