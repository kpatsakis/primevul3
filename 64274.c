meter_update(struct meter *meter, const struct ofputil_meter_config *config)
{
    free(meter->bands);

    meter->flags = config->flags;
    meter->n_bands = config->n_bands;
    meter->bands = xmemdup(config->bands,
                           config->n_bands * sizeof *meter->bands);
}
