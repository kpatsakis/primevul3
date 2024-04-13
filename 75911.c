bool TypedUrlModelAssociator::CryptoReadyIfNecessary() {
  sync_api::ReadTransaction trans(sync_service_->GetUserShare());
  syncable::ModelTypeSet encrypted_types;
  encrypted_types = sync_api::GetEncryptedTypes(&trans);
  return encrypted_types.count(syncable::TYPED_URLS) == 0 ||
         sync_service_->IsCryptographerReady(&trans);
}
