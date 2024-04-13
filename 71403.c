static int ib_uverbs_open(struct inode *inode, struct file *filp)
{
	struct ib_uverbs_device *dev;
	struct ib_uverbs_file *file;
	struct ib_device *ib_dev;
	int ret;
	int module_dependent;
	int srcu_key;

	dev = container_of(inode->i_cdev, struct ib_uverbs_device, cdev);
	if (!atomic_inc_not_zero(&dev->refcount))
		return -ENXIO;

	get_device(&dev->dev);
	srcu_key = srcu_read_lock(&dev->disassociate_srcu);
	mutex_lock(&dev->lists_mutex);
	ib_dev = srcu_dereference(dev->ib_dev,
				  &dev->disassociate_srcu);
	if (!ib_dev) {
		ret = -EIO;
		goto err;
	}

	/* In case IB device supports disassociate ucontext, there is no hard
	 * dependency between uverbs device and its low level device.
	 */
	module_dependent = !(ib_dev->ops.disassociate_ucontext);

	if (module_dependent) {
		if (!try_module_get(ib_dev->owner)) {
			ret = -ENODEV;
			goto err;
		}
	}

	file = kzalloc(sizeof(*file), GFP_KERNEL);
	if (!file) {
		ret = -ENOMEM;
		if (module_dependent)
			goto err_module;

		goto err;
	}

	file->device	 = dev;
	kref_init(&file->ref);
	mutex_init(&file->ucontext_lock);

	spin_lock_init(&file->uobjects_lock);
	INIT_LIST_HEAD(&file->uobjects);
	init_rwsem(&file->hw_destroy_rwsem);
	mutex_init(&file->umap_lock);
	INIT_LIST_HEAD(&file->umaps);

	filp->private_data = file;
	list_add_tail(&file->list, &dev->uverbs_file_list);
	mutex_unlock(&dev->lists_mutex);
	srcu_read_unlock(&dev->disassociate_srcu, srcu_key);

	setup_ufile_idr_uobject(file);

	return nonseekable_open(inode, filp);

err_module:
	module_put(ib_dev->owner);

err:
	mutex_unlock(&dev->lists_mutex);
	srcu_read_unlock(&dev->disassociate_srcu, srcu_key);
	if (atomic_dec_and_test(&dev->refcount))
		ib_uverbs_comp_dev(dev);

	put_device(&dev->dev);
	return ret;
}
