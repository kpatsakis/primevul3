static void hidg_disable(struct usb_function *f)
{
	struct f_hidg *hidg = func_to_hidg(f);
	struct f_hidg_req_list *list, *next;
	unsigned long flags;

	usb_ep_disable(hidg->in_ep);
	usb_ep_disable(hidg->out_ep);

	spin_lock_irqsave(&hidg->read_spinlock, flags);
	list_for_each_entry_safe(list, next, &hidg->completed_out_req, list) {
		free_ep_req(hidg->out_ep, list->req);
		list_del(&list->list);
		kfree(list);
	}
	spin_unlock_irqrestore(&hidg->read_spinlock, flags);

	spin_lock_irqsave(&hidg->write_spinlock, flags);
	if (!hidg->write_pending) {
		free_ep_req(hidg->in_ep, hidg->req);
		hidg->write_pending = 1;
	}

	hidg->req = NULL;
	spin_unlock_irqrestore(&hidg->write_spinlock, flags);
}
