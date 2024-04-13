OPJ_BOOL opj_pi_next(opj_pi_iterator_t * pi)
{
    switch (pi->poc.prg) {
    case OPJ_LRCP:
        return opj_pi_next_lrcp(pi);
    case OPJ_RLCP:
        return opj_pi_next_rlcp(pi);
    case OPJ_RPCL:
        return opj_pi_next_rpcl(pi);
    case OPJ_PCRL:
        return opj_pi_next_pcrl(pi);
    case OPJ_CPRL:
        return opj_pi_next_cprl(pi);
    case OPJ_PROG_UNKNOWN:
        return OPJ_FALSE;
    }

    return OPJ_FALSE;
}
