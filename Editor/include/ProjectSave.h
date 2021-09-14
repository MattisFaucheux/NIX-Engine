#pragma once

#define PROJECT_SAVE_PATH "Resources\\ProjectSaves\\projectSave.xml"

namespace NIX {
	namespace Core {
		struct SaveElement;
	}
}

namespace NIX::Editor
{
	class ProjectSave
	{
	private:
		
	public:
		ProjectSave() = default;
		
		void Serialize();

		void Deserialize();

	private:

		void SerializeShippingSettings(Core::SaveElement& parent);
		void DeserializeShippingSettings(Core::SaveElement& parent);

		void SerializeTags(Core::SaveElement& parent);
		void DeserializeTags(Core::SaveElement& parent);
		
	};
}
