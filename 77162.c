void NetworkActionPredictor::Shutdown() {
  db_->OnPredictorDestroyed();
}
