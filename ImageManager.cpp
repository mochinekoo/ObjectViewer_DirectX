#include "ImageManager.h"
#include <vector>
#include "Image.h"
#include <filesystem>

namespace ImageManager {
	std::vector<Image*> imageList_;
}

void ImageManager::Init() {
	imageList_.clear();
}

int ImageManager::Load(const std::string& fileName) {
	for (int i = 0; i < imageList_.size(); i++) {
		Image* image = imageList_[i];
		if (image->GetFileName() == fileName) {
			return i;
		}
	}

	if (!std::filesystem::exists(fileName)) {
		return -1;
	}

	Image* image = new Image(fileName);
	image->Init();
	imageList_.push_back(image);

	return imageList_.size() - 1;
}

void ImageManager::Draw(const int id) {
	imageList_[id]->Update();
	imageList_[id]->Draw();
}

Transform ImageManager::GetTransform(const int id) {
	return imageList_[id]->GetTransform();
}

void ImageManager::SetTransform(const int id, const Transform& transform) {
	imageList_[id]->SetTransform(transform);
}
