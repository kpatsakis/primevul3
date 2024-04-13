bool OmniboxViewWin::GetAcceleratorForCommandId(
    int command_id,
    ui::Accelerator* accelerator) {
  return parent_view_->GetWidget()->GetAccelerator(command_id, accelerator);
}
