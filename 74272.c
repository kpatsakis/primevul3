static int amd_gpio_irq_set_type(struct irq_data *d, unsigned int type)
{
	int ret = 0;
	u32 pin_reg;
	unsigned long flags;
	struct gpio_chip *gc = irq_data_get_irq_chip_data(d);
	struct amd_gpio *gpio_dev = gpiochip_get_data(gc);

	spin_lock_irqsave(&gpio_dev->lock, flags);
	pin_reg = readl(gpio_dev->base + (d->hwirq)*4);

	switch (type & IRQ_TYPE_SENSE_MASK) {
	case IRQ_TYPE_EDGE_RISING:
		pin_reg &= ~BIT(LEVEL_TRIG_OFF);
		pin_reg &= ~(ACTIVE_LEVEL_MASK << ACTIVE_LEVEL_OFF);
		pin_reg |= ACTIVE_HIGH << ACTIVE_LEVEL_OFF;
		pin_reg |= DB_TYPE_REMOVE_GLITCH << DB_CNTRL_OFF;
		irq_set_handler_locked(d, handle_edge_irq);
		break;

	case IRQ_TYPE_EDGE_FALLING:
		pin_reg &= ~BIT(LEVEL_TRIG_OFF);
		pin_reg &= ~(ACTIVE_LEVEL_MASK << ACTIVE_LEVEL_OFF);
		pin_reg |= ACTIVE_LOW << ACTIVE_LEVEL_OFF;
		pin_reg |= DB_TYPE_REMOVE_GLITCH << DB_CNTRL_OFF;
		irq_set_handler_locked(d, handle_edge_irq);
		break;

	case IRQ_TYPE_EDGE_BOTH:
		pin_reg &= ~BIT(LEVEL_TRIG_OFF);
		pin_reg &= ~(ACTIVE_LEVEL_MASK << ACTIVE_LEVEL_OFF);
		pin_reg |= BOTH_EADGE << ACTIVE_LEVEL_OFF;
		pin_reg |= DB_TYPE_REMOVE_GLITCH << DB_CNTRL_OFF;
		irq_set_handler_locked(d, handle_edge_irq);
		break;

	case IRQ_TYPE_LEVEL_HIGH:
		pin_reg |= LEVEL_TRIGGER << LEVEL_TRIG_OFF;
		pin_reg &= ~(ACTIVE_LEVEL_MASK << ACTIVE_LEVEL_OFF);
		pin_reg |= ACTIVE_HIGH << ACTIVE_LEVEL_OFF;
		pin_reg &= ~(DB_CNTRl_MASK << DB_CNTRL_OFF);
		pin_reg |= DB_TYPE_PRESERVE_LOW_GLITCH << DB_CNTRL_OFF;
		irq_set_handler_locked(d, handle_level_irq);
		break;

	case IRQ_TYPE_LEVEL_LOW:
		pin_reg |= LEVEL_TRIGGER << LEVEL_TRIG_OFF;
		pin_reg &= ~(ACTIVE_LEVEL_MASK << ACTIVE_LEVEL_OFF);
		pin_reg |= ACTIVE_LOW << ACTIVE_LEVEL_OFF;
		pin_reg &= ~(DB_CNTRl_MASK << DB_CNTRL_OFF);
		pin_reg |= DB_TYPE_PRESERVE_HIGH_GLITCH << DB_CNTRL_OFF;
		irq_set_handler_locked(d, handle_level_irq);
		break;

	case IRQ_TYPE_NONE:
		break;

	default:
		dev_err(&gpio_dev->pdev->dev, "Invalid type value\n");
		ret = -EINVAL;
	}

	pin_reg |= CLR_INTR_STAT << INTERRUPT_STS_OFF;
	writel(pin_reg, gpio_dev->base + (d->hwirq)*4);
	spin_unlock_irqrestore(&gpio_dev->lock, flags);

	return ret;
}
