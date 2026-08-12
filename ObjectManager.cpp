#include "ObjectManager.h"

namespace ObjectManager {
	std::vector<BaseObject*> objectList_;
}

void ObjectManager::Init() {
	objectList_.clear();
}

void ObjectManager::Update() {
	for (int i = 0; i < objectList_.size(); i++) {
		BaseObject* object = objectList_[i];
		if (object == nullptr) continue;
		if (object->IsDead()) {
			RemoveObject(object);
		}
		else {
			object->Update();
			object->Draw();
		}
	}
}

BaseObject* ObjectManager::AddObject(BaseObject* object) {
	objectList_.push_back(object);
	object->Init();
	return object;
}

void ObjectManager::RemoveObject(BaseObject* object) {
	for (auto it = objectList_.begin(); it != objectList_.end(); ++it) {
		if (*it == object) {
			delete* it;
			it = objectList_.erase(it);
		}
		else {
			it++;
		}
	}
}

std::vector<BaseObject*>& ObjectManager::GetAllObject() {
	return objectList_;
}

void ObjectManager::ClearAllObject() {
	objectList_.clear();
}
