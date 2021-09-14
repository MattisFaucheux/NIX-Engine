
Start = function(gameObject)

	pos = Vector3(0, -10, 0)

	cb = gameObject:GetComponentCollisionBox()

	
	gameObject.name = "Hello World!"
	t = gameObject:GetComponentTransform()
	t.WorldPosition = pos
	Log.DebugLog(gameObject.name)
end

Update = function(gameObject, deltaTime)

	move = Vector3(0, 1 * deltaTime * 2, 0)
	t = gameObject:GetComponentTransform()
	
	if Input.IsKeyboardInputPressed("W") or Input.IsKeyboardInputRepeated("W") then
		pos = t.WorldPosition + move
		t.WorldPosition = pos
	elseif Input.IsKeyboardInputPressed("S") or Input.IsKeyboardInputRepeated("S") then
		pos = t.WorldPosition - move
		t.WorldPosition = pos
	end
end


OnTriggerEnter = function(gameObject, other)

	msg = "OnTriggerEnter " .. gameObject.name
	Log.DebugLog(msg)
	
end

OnTriggerExit = function(gameObject, other)

	msg = "OnTriggerExit " .. gameObject.name
	Log.DebugLog(msg)

end

OnCollisionEnter = function(gameObject, other)

	msg = "OnCollisionEnter " .. gameObject.name
	Log.DebugLog(msg)

end

OnCollisionExit = function(gameObject, other)

	msg = "OnCollisionExit " .. gameObject.name
	Log.DebugLog(msg)

end


