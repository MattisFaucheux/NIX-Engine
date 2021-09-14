#pragma once
#include <string_view>

#include "UINodeComponent.h"

namespace NIX::Editor
{
	class RigidBody;

	class UINodeRigidBody : UINodeComponent
	{
	private:		
		float m_mass = 1.0f;
		bool m_isGravity = true;
		bool m_kinematic = true;

		bool m_freezePosX = false;
		bool m_freezePosY = false;
		bool m_freezePosZ = false;
		bool m_freezeRotX = false;
		bool m_freezeRotY = false;
		bool m_freezeRotZ = false;

		float m_linearDamping = 0.0f;
		float m_angularDamping = 0.0f;
		float m_maxLinearVelocity = 0.0f;
		float m_maxAngularVelocity = 0.0f;

		float m_massInertiaTensor[3] = { 0.0f, 0.0f, 0.0f };
		float m_centerOfMass[3] = { 0.0f, 0.0f, 0.0f };

		bool m_isConstraintsOpen = true;
	public:
		void UpdateComponentProperties(GameObject* obj) override;

		void UpdateComponent() override;
	private:
		void UpdateComponentNode(bool& isComponentOpen) override;
		

		void UpdateMass(RigidBody* rigidBodyComponent);
		void UpdateIsGravity(RigidBody* rigidBodyComponent);
		void UpdateIsKinematic(RigidBody* rigidBodyComponent);
		void UpdateConstraints(RigidBody* rigidBodyComponent);
		void UpdateLinearDamping(RigidBody* rigidBodyComponent);
		void UpdateAngularDamping(RigidBody* rigidBodyComponent);
		void UpdateMaxLinearVelocity(RigidBody* rigidBodyComponent);
		void UpdateMaxAngularVelocity(RigidBody* rigidBodyComponent);
		void UpdateMaxInertiaTensor(RigidBody* rigidBodyComponent);
		void UpdateLocalCenterOfMass();

		void UpdateConstraintsNode(RigidBody* rigidBodyComponent);
		void UpdateFreezePositionNode(RigidBody* rigidBodyComponent);
		void UpdateFreezeRotationNode(RigidBody* rigidBodyComponent);
		void SetTextConstraints(std::string_view text);
	};
}
