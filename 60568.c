static RTSPTransportField *find_transport(RTSPMessageHeader *h, enum RTSPLowerTransport lower_transport)
{
    RTSPTransportField *th;
    int i;

    for(i=0;i<h->nb_transports;i++) {
        th = &h->transports[i];
        if (th->lower_transport == lower_transport)
            return th;
    }
    return NULL;
}
