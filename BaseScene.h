#pragma once
#include <string>

class BaseScene {
private:
protected:
	std::string name_;
public:

	BaseScene(const std::string& sceneName) {
		name_ = sceneName;
	}

	BaseScene() {}

	std::string GetName() const {  return name_; }

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

};