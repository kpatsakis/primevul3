static void port_over_current_notify(struct usb_port *port_dev)
{
	char *envp[3];
	struct device *hub_dev;
	char *port_dev_path;

	sysfs_notify(&port_dev->dev.kobj, NULL, "over_current_count");

	hub_dev = port_dev->dev.parent;

	if (!hub_dev)
		return;

	port_dev_path = kobject_get_path(&port_dev->dev.kobj, GFP_KERNEL);
	if (!port_dev_path)
		return;

	envp[0] = kasprintf(GFP_KERNEL, "OVER_CURRENT_PORT=%s", port_dev_path);
	if (!envp[0])
		goto exit_path;

	envp[1] = kasprintf(GFP_KERNEL, "OVER_CURRENT_COUNT=%u",
			port_dev->over_current_count);
	if (!envp[1])
		goto exit;

	envp[2] = NULL;
	kobject_uevent_env(&hub_dev->kobj, KOBJ_CHANGE, envp);

	kfree(envp[1]);
exit:
	kfree(envp[0]);
exit_path:
	kfree(port_dev_path);
}
