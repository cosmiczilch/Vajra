#include "Vajra/Engine/AssetLibrary/Assets/TextureAsset.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/TextureLoader/TextureLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

AssetType TextureAsset::assetType = ASSET_TYPE_GL_TEXTURE;

TextureAsset::TextureAsset() : Asset() {
	this->init();
}

TextureAsset::TextureAsset(std::string urlOfTexture) : Asset(urlOfTexture) {
	this->init();
}

TextureAsset::~TextureAsset() {
}

void TextureAsset::init() {
	this->textureBytes = nullptr;
}

void TextureAsset::destroy() {
	if (this->textureBytes != nullptr) {
		free(this->textureBytes);
	}
}

AssetType TextureAsset::GetAssetType() {
	return TextureAsset::assetType;
}

void TextureAsset::LoadAsset() {
	ASSERT(this->GetUrl() != "", "Url set when trying to load texture asset");

	FRAMEWORK->GetLogger()->dbglog("\nLoading texture asset from url: %s", this->GetFilePathToTexture().c_str());

	// TODO [Implement] Move loadGLTextureFromPNG into a Framework class/namespace
    // Load image
    this->textureGLHandle = loadGLTextureFromPNG(this->GetFilePathToTexture().c_str(), &(this->textureBytes));
}

std::string TextureAsset::GetFilePathToTexture() {
	return this->GetUrl();
}