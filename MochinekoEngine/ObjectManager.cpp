#include "ObjectManager.h"
#include <Windows.h>
#include "SphereCollider.h"
#include "BoxCollider.h"

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
		if (object->GetParent() == nullptr) {
			object->UpdateTransform();
		}
	}

	for (int i = 0; i < objectList_.size(); i++) {
		for (int a = 1; a < objectList_.size(); a++) {
			BaseObject* object = objectList_[i];
			if (object == nullptr) continue;
			BaseObject* object1 = objectList_[a];

			if (object->GetDrawOrder() > object1->GetDrawOrder()) {
				std::swap(objectList_[i], objectList_[a]);
			}
		}
	}

	for (int i = 0; i < objectList_.size(); i++) {
		BaseObject* objectA = objectList_[i];
		auto& colliderListA = objectA->GetColliderList();
		for (int a = i + 1; a < objectList_.size(); a++) {
			BaseObject* objectB = objectList_[a];
			auto& colliderListB = objectB->GetColliderList();

			if (objectA == objectB) continue;
			bool isHit = false;

			for (int aCol = 0; aCol < colliderListA.size(); aCol++) {
				BaseCollider* colliderA = colliderListA[aCol];
				SphereCollider* sphereColA = dynamic_cast<SphereCollider*>(colliderA);
				BoxCollider* boxColA = dynamic_cast<BoxCollider*>(colliderA);

				for (int bCol = 0; bCol < colliderListB.size(); bCol++) {
					BaseCollider* colliderB = colliderListB[bCol];
					SphereCollider* sphereColB = dynamic_cast<SphereCollider*>(colliderB);
					BoxCollider* boxColB = dynamic_cast<BoxCollider*>(colliderB);

					if (colliderA == colliderB) continue;
					if (colliderA->GetColliderType() == ColliderType::SPHERE && colliderB->GetColliderType() == ColliderType::SPHERE) {
						if (SphereCollider::IsHitSphereSphere(sphereColA, sphereColB)) {
							// OutputDebugString(L"Hit: Sphere VS Sphere \n");
							isHit = true;
							continue;
						}
					}
					if (colliderA->GetColliderType() == ColliderType::BOX && colliderB->GetColliderType() == ColliderType::BOX) {
						if (BoxCollider::IsHitBoxBox(boxColA, boxColB)) {
							// OutputDebugString(L"Hit: Box VS Box \n");
							isHit = true;
							continue;
						}
					}
				}
			}

			if (isHit) {
				OutputDebugString(L"Hit! \n");
				continue;
			}
		}
	}


	for (int i = 0; i < objectList_.size(); i++) {
		BaseObject* object = objectList_[i];
		auto& colliderList = object->GetColliderList();
		if (object == nullptr) continue;
		if (object->IsDead()) {
			RemoveObject(object);
		}
		else {
			object->Update();
			object->Draw();

			for (int a = 0; a < colliderList.size(); a++) {
				BaseCollider* colliderA = colliderList[a];
				colliderA->Update();
				colliderA->Draw();
			}

			if (object->IsShowImGUI()) {
				object->DrawImGUI();
			}
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
