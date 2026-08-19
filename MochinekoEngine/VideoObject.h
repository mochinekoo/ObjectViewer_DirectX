#pragma once
#include "BaseObject.h"
#include "VideoData.h"

class VideoObject : public BaseObject {
private:
	VideoData* videoData_;
public:

	VideoObject(const std::string& fileName)
		: BaseObject("VideoTexture") {
		videoData_ = nullptr;
		videoData_ = new VideoData(fileName);
	};

	~VideoObject() {
		delete videoData_;
	};

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawImGUI() override;
	void Release() override;

	std::string GetFileName() const { return videoData_->GetFileName(); }
};