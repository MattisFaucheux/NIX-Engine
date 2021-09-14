speed = 15
jumpForce = 50
rotationSpeed = 70

Start = function(gameObject)

end

Update = function(gameObject, deltaTime)

	RotatePlayer(gameObject, deltaTime)
	Move(gameObject, deltaTime)
	Jump(gameObject, deltaTime)

end

RotatePlayer = function(gameObject, deltaTime)

	t = gameObject:GetComponentTransform()
	
	if Input.IsKeyboardInputPressed("RIGHT") or Input.IsKeyboardInputRepeated("RIGHT") then
		
		v = Vector3(0, 1, 0)
		t:RotateLocal(Quaternion.New(rotationSpeed * deltaTime, v))
		
	end
		
	if Input.IsKeyboardInputPressed("LEFT") or Input.IsKeyboardInputRepeated("LEFT") then
		
		v = Vector3(0, -1, 0)
		t:RotateLocal(Quaternion.New(rotationSpeed * deltaTime, v))
		
	end


end

Jump = function(gameObject, deltaTime)

	t = gameObject:GetComponentTransform()

	collisionCapsule = gameObject:GetComponentCollisionCapsule()
	
	if collisionCapsule ~= nil then
	
		origin = t.WorldPosition - Vector3(0, (collisionCapsule.Height) - 0.1, 0)
		rHitBuff = Raycast.LaunchRaycast(origin, Vector3(0, -1, 0), 0.5)
		isGrounded = rHitBuff:HasBlock()
		
		
		if Input.IsKeyboardInputPressed("SPACE")then
			if isGrounded then
				rb = gameObject:GetComponentRigidBody()
				
				canJump = false
				jump = t:GetLocalUp()
				jump:Multiply(Vector3(jumpForce, jumpForce, jumpForce))
				forceMode = "Force"
				
				rb:AddForce(jump, forceMode)
			end
		end
	
	end

end

Move = function(gameObject, deltaTime)

	rb = gameObject:GetComponentRigidBody()
	t = gameObject:GetComponentTransform()
	move = Vector3(0, 0, 0)
	
	moveSpeed = Vector3(speed * deltaTime, speed * deltaTime, speed * deltaTime)

	if Input.IsKeyboardInputPressed("W") or Input.IsKeyboardInputRepeated("W") then
		temp = t:GetLocalForward()
		move:Add(temp)
	end	
	
	if Input.IsKeyboardInputPressed("S") or Input.IsKeyboardInputRepeated("S") then
		temp = t:GetLocalForward()
		temp:Multiply(Vector3(-1, -1, -1))
		move:Add(temp)
	end
	
	if Input.IsKeyboardInputPressed("D") or Input.IsKeyboardInputRepeated("D") then
		temp = t:GetLocalRight()
		move:Add(temp)
	end
	
	if Input.IsKeyboardInputPressed("A") or Input.IsKeyboardInputRepeated("A") then
		temp = t:GetLocalRight()
		temp:Multiply(Vector3(-1, -1, -1))
		move:Add(temp)
	end
	
	move:Multiply(moveSpeed)
	move:Add(t.WorldPosition)
	
	if rb ~= nil then
		rb:SetPosition(move)
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