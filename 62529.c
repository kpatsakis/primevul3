static int machine_constraints_voltage(struct regulator_dev *rdev,
	struct regulation_constraints *constraints)
{
	const struct regulator_ops *ops = rdev->desc->ops;
	int ret;

	/* do we need to apply the constraint voltage */
	if (rdev->constraints->apply_uV &&
	    rdev->constraints->min_uV == rdev->constraints->max_uV) {
		int current_uV = _regulator_get_voltage(rdev);
		if (current_uV < 0) {
			rdev_err(rdev,
				 "failed to get the current voltage(%d)\n",
				 current_uV);
			return current_uV;
		}
		if (current_uV < rdev->constraints->min_uV ||
		    current_uV > rdev->constraints->max_uV) {
			ret = _regulator_do_set_voltage(
				rdev, rdev->constraints->min_uV,
				rdev->constraints->max_uV);
			if (ret < 0) {
				rdev_err(rdev,
					"failed to apply %duV constraint(%d)\n",
					rdev->constraints->min_uV, ret);
				return ret;
			}
		}
	}

	/* constrain machine-level voltage specs to fit
	 * the actual range supported by this regulator.
	 */
	if (ops->list_voltage && rdev->desc->n_voltages) {
		int	count = rdev->desc->n_voltages;
		int	i;
		int	min_uV = INT_MAX;
		int	max_uV = INT_MIN;
		int	cmin = constraints->min_uV;
		int	cmax = constraints->max_uV;

		/* it's safe to autoconfigure fixed-voltage supplies
		   and the constraints are used by list_voltage. */
		if (count == 1 && !cmin) {
			cmin = 1;
			cmax = INT_MAX;
			constraints->min_uV = cmin;
			constraints->max_uV = cmax;
		}

		/* voltage constraints are optional */
		if ((cmin == 0) && (cmax == 0))
			return 0;

		/* else require explicit machine-level constraints */
		if (cmin <= 0 || cmax <= 0 || cmax < cmin) {
			rdev_err(rdev, "invalid voltage constraints\n");
			return -EINVAL;
		}

		/* initial: [cmin..cmax] valid, [min_uV..max_uV] not */
		for (i = 0; i < count; i++) {
			int	value;

			value = ops->list_voltage(rdev, i);
			if (value <= 0)
				continue;

			/* maybe adjust [min_uV..max_uV] */
			if (value >= cmin && value < min_uV)
				min_uV = value;
			if (value <= cmax && value > max_uV)
				max_uV = value;
		}

		/* final: [min_uV..max_uV] valid iff constraints valid */
		if (max_uV < min_uV) {
			rdev_err(rdev,
				 "unsupportable voltage constraints %u-%uuV\n",
				 min_uV, max_uV);
			return -EINVAL;
		}

		/* use regulator's subset of machine constraints */
		if (constraints->min_uV < min_uV) {
			rdev_dbg(rdev, "override min_uV, %d -> %d\n",
				 constraints->min_uV, min_uV);
			constraints->min_uV = min_uV;
		}
		if (constraints->max_uV > max_uV) {
			rdev_dbg(rdev, "override max_uV, %d -> %d\n",
				 constraints->max_uV, max_uV);
			constraints->max_uV = max_uV;
		}
	}

	return 0;
}
