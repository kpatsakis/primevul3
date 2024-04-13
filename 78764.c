DelegatedFrameHost::~DelegatedFrameHost() {
  ImageTransportFactory::GetInstance()->RemoveObserver(this);

  if (!surface_id_.is_null())
    surface_factory_->Destroy(surface_id_);
  if (resource_collection_.get())
    resource_collection_->SetClient(NULL);

  DCHECK(!vsync_manager_);
}
