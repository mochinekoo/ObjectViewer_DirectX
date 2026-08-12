#pragma once
#include "BaseObject.h"
#include <string>
#include "Texture.h"

class Image : public BaseObject {
private:
	Texture* texture_;
public:

	Image(const std::string& fileName)
		: BaseObject("Image") {
		texture_ = new Texture(fileName);
	}

	~Image() {
		delete texture_;
	}

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	std::string GetFileName() const { return texture_->GetFileName(); }
};

