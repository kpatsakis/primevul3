regulator_register(const struct regulator_desc *regulator_desc,
		   const struct regulator_config *config)
{
	const struct regulation_constraints *constraints = NULL;
	const struct regulator_init_data *init_data;
	static atomic_t regulator_no = ATOMIC_INIT(0);
	struct regulator_dev *rdev;
	struct device *dev;
	int ret, i;
	const char *supply = NULL;

	if (regulator_desc == NULL || config == NULL)
		return ERR_PTR(-EINVAL);

	dev = config->dev;
	WARN_ON(!dev);

	if (regulator_desc->name == NULL || regulator_desc->ops == NULL)
		return ERR_PTR(-EINVAL);

	if (regulator_desc->type != REGULATOR_VOLTAGE &&
	    regulator_desc->type != REGULATOR_CURRENT)
		return ERR_PTR(-EINVAL);

	/* Only one of each should be implemented */
	WARN_ON(regulator_desc->ops->get_voltage &&
		regulator_desc->ops->get_voltage_sel);
	WARN_ON(regulator_desc->ops->set_voltage &&
		regulator_desc->ops->set_voltage_sel);

	/* If we're using selectors we must implement list_voltage. */
	if (regulator_desc->ops->get_voltage_sel &&
	    !regulator_desc->ops->list_voltage) {
		return ERR_PTR(-EINVAL);
	}
	if (regulator_desc->ops->set_voltage_sel &&
	    !regulator_desc->ops->list_voltage) {
		return ERR_PTR(-EINVAL);
	}

	rdev = kzalloc(sizeof(struct regulator_dev), GFP_KERNEL);
	if (rdev == NULL)
		return ERR_PTR(-ENOMEM);

	init_data = regulator_of_get_init_data(dev, regulator_desc,
					       &rdev->dev.of_node);
	if (!init_data) {
		init_data = config->init_data;
		rdev->dev.of_node = of_node_get(config->of_node);
	}

	mutex_lock(&regulator_list_mutex);

	mutex_init(&rdev->mutex);
	rdev->reg_data = config->driver_data;
	rdev->owner = regulator_desc->owner;
	rdev->desc = regulator_desc;
	if (config->regmap)
		rdev->regmap = config->regmap;
	else if (dev_get_regmap(dev, NULL))
		rdev->regmap = dev_get_regmap(dev, NULL);
	else if (dev->parent)
		rdev->regmap = dev_get_regmap(dev->parent, NULL);
	INIT_LIST_HEAD(&rdev->consumer_list);
	INIT_LIST_HEAD(&rdev->list);
	BLOCKING_INIT_NOTIFIER_HEAD(&rdev->notifier);
	INIT_DELAYED_WORK(&rdev->disable_work, regulator_disable_work);

	/* preform any regulator specific init */
	if (init_data && init_data->regulator_init) {
		ret = init_data->regulator_init(rdev->reg_data);
		if (ret < 0)
			goto clean;
	}

	/* register with sysfs */
	rdev->dev.class = &regulator_class;
	rdev->dev.parent = dev;
	dev_set_name(&rdev->dev, "regulator.%d",
		     atomic_inc_return(&regulator_no) - 1);
	ret = device_register(&rdev->dev);
	if (ret != 0) {
		put_device(&rdev->dev);
		goto clean;
	}

	dev_set_drvdata(&rdev->dev, rdev);

	if (config->ena_gpio && gpio_is_valid(config->ena_gpio)) {
		ret = regulator_ena_gpio_request(rdev, config);
		if (ret != 0) {
			rdev_err(rdev, "Failed to request enable GPIO%d: %d\n",
				 config->ena_gpio, ret);
			goto wash;
		}

		if (config->ena_gpio_flags & GPIOF_OUT_INIT_HIGH)
			rdev->ena_gpio_state = 1;

		if (config->ena_gpio_invert)
			rdev->ena_gpio_state = !rdev->ena_gpio_state;
	}

	/* set regulator constraints */
	if (init_data)
		constraints = &init_data->constraints;

	ret = set_machine_constraints(rdev, constraints);
	if (ret < 0)
		goto scrub;

	/* add attributes supported by this regulator */
	ret = add_regulator_attributes(rdev);
	if (ret < 0)
		goto scrub;

	if (init_data && init_data->supply_regulator)
		supply = init_data->supply_regulator;
	else if (regulator_desc->supply_name)
		supply = regulator_desc->supply_name;

	if (supply) {
		struct regulator_dev *r;

		r = regulator_dev_lookup(dev, supply, &ret);

		if (ret == -ENODEV) {
			/*
			 * No supply was specified for this regulator and
			 * there will never be one.
			 */
			ret = 0;
			goto add_dev;
		} else if (!r) {
			dev_err(dev, "Failed to find supply %s\n", supply);
			ret = -EPROBE_DEFER;
			goto scrub;
		}

		ret = set_supply(rdev, r);
		if (ret < 0)
			goto scrub;

		/* Enable supply if rail is enabled */
		if (_regulator_is_enabled(rdev)) {
			ret = regulator_enable(rdev->supply);
			if (ret < 0)
				goto scrub;
		}
	}

add_dev:
	/* add consumers devices */
	if (init_data) {
		for (i = 0; i < init_data->num_consumer_supplies; i++) {
			ret = set_consumer_device_supply(rdev,
				init_data->consumer_supplies[i].dev_name,
				init_data->consumer_supplies[i].supply);
			if (ret < 0) {
				dev_err(dev, "Failed to set supply %s\n",
					init_data->consumer_supplies[i].supply);
				goto unset_supplies;
			}
		}
	}

	list_add(&rdev->list, &regulator_list);

	rdev_init_debugfs(rdev);
out:
	mutex_unlock(&regulator_list_mutex);
	return rdev;

unset_supplies:
	unset_regulator_supplies(rdev);

scrub:
	if (rdev->supply)
		_regulator_put(rdev->supply);
	regulator_ena_gpio_free(rdev);
	kfree(rdev->constraints);
wash:
	device_unregister(&rdev->dev);
	/* device core frees rdev */
	rdev = ERR_PTR(ret);
	goto out;

clean:
	kfree(rdev);
	rdev = ERR_PTR(ret);
	goto out;
}
