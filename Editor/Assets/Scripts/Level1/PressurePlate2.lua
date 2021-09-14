Start = function(gameObject)

end

Update = function(gameObject, deltaTime)

end

RotateBridge = function(gameObject)

	model = gameObject:GetComponentModel()
	if model.MaterialPath == "Assets\\Materials\\greenMaterial.mat" then
	
		go = Scene.GetGameObjectWithTag("Bridge2", 0)

		t = go:GetComponentTransform()
		v = Vector3(0, 0, 1)

		t:RotateLocal(Quaternion.New(80, v))
		
	else
	
		go = Scene.GetGameObjectWithTag("Bridge2", 0)
		
		t = go:GetComponentTransform()
		v = Vector3(0, 0, 1)

		t:RotateLocal(Quaternion.New(-80, v))
		
	end

end

OnTriggerEnter = function(gameObject, other)
	
	tag = other.tag

	if tag == "CubeObject" then
		
		model = gameObject:GetComponentModel()
		model.MaterialPath = "Assets\\Materials\\greenMaterial.mat"
		
		RotateBridge(gameObject)
		
	end

end

OnTriggerExit = function(gameObject, other)

	tag = other.tag

	if tag == "CubeObject" then
		
		model = gameObject:GetComponentModel()
		model.MaterialPath = "Assets\\Materials\\redMaterial.mat"
		
		RotateBridge(gameObject)
		
	end

end

OnCollisionEnter = function(gameObject, other)

end

OnCollisionExit = function(gameObject, other)

end


