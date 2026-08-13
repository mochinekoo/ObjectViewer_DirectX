#pragma once
#include "BaseScene.h"
#include "FBX.h"

class DebugScene : public BaseScene {
private:
	int counter_;
public:

	DebugScene()
		: BaseScene("DebugScene") {
		counter_ = 0;
	};

	~DebugScene() {
	};

	void Init() override;
	void Update() override;
	void Draw() override;

};

