rotationSpeed = 70

Start = function(gameObject)

	cam = gameObject:GetComponentCamera()
	cam.Fov = 75

end

Update = function(gameObject, deltaTime)

	t = gameObject:GetComponentTransform()
	
	if Input.IsKeyboardInputPressed("UP") or Input.IsKeyboardInputRepeated("UP") then
		
		v = Vector3(-1, 0, 0)
		t:RotateLocal(Quaternion.New(rotationSpeed * deltaTime, v))
		
	elseif Input.IsKeyboardInputPressed("DOWN") or Input.IsKeyboardInputRepeated("DOWN") then
		
		v = Vector3(1, 0, 0)
		t:RotateLocal(Quaternion.New(rotationSpeed * deltaTime, v))
		
	end

end

OnTriggerEnter = function(gameObject, other)

end

OnTriggerExit = function(gameObject, other)

end

OnCollisionEnter = function(gameObject, other)

end

OnCollisionExit = function(gameObject, other)

end