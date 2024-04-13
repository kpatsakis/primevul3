nvmet_fc_delete_target_queue(struct nvmet_fc_tgt_queue *queue)
{
	struct nvmet_fc_tgtport *tgtport = queue->assoc->tgtport;
	struct nvmet_fc_fcp_iod *fod = queue->fod;
	struct nvmet_fc_defer_fcp_req *deferfcp, *tempptr;
	unsigned long flags;
	int i, writedataactive;
	bool disconnect;

	disconnect = atomic_xchg(&queue->connected, 0);

	spin_lock_irqsave(&queue->qlock, flags);
	/* about outstanding io's */
	for (i = 0; i < queue->sqsize; fod++, i++) {
		if (fod->active) {
			spin_lock(&fod->flock);
			fod->abort = true;
			writedataactive = fod->writedataactive;
			spin_unlock(&fod->flock);
			/*
			 * only call lldd abort routine if waiting for
			 * writedata. other outstanding ops should finish
			 * on their own.
			 */
			if (writedataactive) {
				spin_lock(&fod->flock);
				fod->aborted = true;
				spin_unlock(&fod->flock);
				tgtport->ops->fcp_abort(
					&tgtport->fc_target_port, fod->fcpreq);
			}
		}
	}

	/* Cleanup defer'ed IOs in queue */
	list_for_each_entry_safe(deferfcp, tempptr, &queue->avail_defer_list,
				req_list) {
		list_del(&deferfcp->req_list);
		kfree(deferfcp);
	}

	for (;;) {
		deferfcp = list_first_entry_or_null(&queue->pending_cmd_list,
				struct nvmet_fc_defer_fcp_req, req_list);
		if (!deferfcp)
			break;

		list_del(&deferfcp->req_list);
		spin_unlock_irqrestore(&queue->qlock, flags);

		tgtport->ops->defer_rcv(&tgtport->fc_target_port,
				deferfcp->fcp_req);

		tgtport->ops->fcp_abort(&tgtport->fc_target_port,
				deferfcp->fcp_req);

		tgtport->ops->fcp_req_release(&tgtport->fc_target_port,
				deferfcp->fcp_req);

		kfree(deferfcp);

		spin_lock_irqsave(&queue->qlock, flags);
	}
	spin_unlock_irqrestore(&queue->qlock, flags);

	flush_workqueue(queue->work_q);

	if (disconnect)
		nvmet_sq_destroy(&queue->nvme_sq);

	nvmet_fc_tgt_q_put(queue);
}
