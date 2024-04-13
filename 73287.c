nvmet_fc_makeconnid(struct nvmet_fc_tgt_assoc *assoc, u16 qid)
{
	return (assoc->association_id | qid);
}
