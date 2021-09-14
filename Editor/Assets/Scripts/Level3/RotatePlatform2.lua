platformTag = "RotatePlatform2"
triggerActorTag = "Player"

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)
--do your code here

end

OnTriggerEnter = function(gameObject, other)
	
	otherTag = other.tag
	
	if otherTag == triggerActorTag then
		
		
		model = gameObject:GetComponentModel()
		model.MaterialPath = "Assets\\Materials\\greenMaterial.mat"
		
		platform1 = Scene.GetGameObjectWithTag(platformTag, 0)
		platform2 = Scene.GetGameObjectWithTag(platformTag, 1)
		
		
		platformTransform1 = platform1:GetComponentTransform()
		platformTransform2 = platform2:GetComponentTransform()
		
		
		platformTransform1.LocalRotation = Quaternion.New(-45, Vector3(1, 0, 0))
		platformTransform2.LocalRotation = Quaternion.New(-45, Vector3(1, 0, 0))
		
	end

end

OnTriggerExit = function(gameObject, other)

	otherTag = other.tag
	
	if otherTag == triggerActorTag then
		
		
		model = gameObject:GetComponentModel()
		model.MaterialPath = "Assets\\Materials\\redMaterial.mat"
		
		platform1 = Scene.GetGameObjectWithTag(platformTag, 0)
		platform2 = Scene.GetGameObjectWithTag(platformTag, 1)
		
		
		platformTransform1 = platform1:GetComponentTransform()
		platformTransform2 = platform2:GetComponentTransform()
		
		
		platformTransform1.LocalRotation = Quaternion.New(45, Vector3(1, 0, 0))
		platformTransform2.LocalRotation = Quaternion.New(45, Vector3(1, 0, 0))

	end

end

OnCollisionEnter = function(gameObject, other)
--do your code here

end

OnCollisionExit = function(gameObject, other)
--do your code here

end