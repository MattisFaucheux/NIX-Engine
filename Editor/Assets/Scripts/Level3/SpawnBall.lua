--declare variables here

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)
	
	t = gameObject:GetComponentTransform()

	if Input.IsKeyboardInputPressed("E") then
		
		origin = t.WorldPosition + (t:GetWorldForward() * Vector3(2, 2, 2))
		rHitBuff = Raycast.LaunchRaycast(origin, t:GetWorldForward():GetNormalize() , 8)
		rayBlock = rHitBuff:HasBlock() or rHitBuff:HasAnyHit()
		
		if rayBlock then
			 
			rHit = rHitBuff:GetBlockInfo()
			 
			if rHit ~= nil then
		
				rayGo = RaycastHit.GetGameObject(rHit)
				
				if rayGo ~= nil then
				
					tag = rayGo.tag
				
					if tag == "Button1" then
		
						ball = Scene.GetGameObjectWithTag("Ball", 0)
						spawnPoint = Scene.GetGameObjectWithTag("SpawnPoint", 0)

						ballTransform = ball:GetComponentTransform()
						spawnPointTransform = spawnPoint:GetComponentTransform()
						
						ballTransform.WorldPosition = spawnPointTransform.WorldPosition
						
						ballRb = ball:GetComponentRigidBody()
						ballRb:ClearForce()
						ballRb:ClearVelocity()
						ballRb.IsGravity = true
						
						ballCs = ball:GetComponentCollisionShape()
						ballCs.IsTrigger = false

						
					elseif tag == "Button2" then
						
						ball = Scene.GetGameObjectWithTag("Ball", 1)
						spawnPoint = Scene.GetGameObjectWithTag("SpawnPoint", 1)
						
						ballTransform = ball:GetComponentTransform()
						spawnPointTransform = spawnPoint:GetComponentTransform()
						
						ballTransform.WorldPosition = spawnPointTransform.WorldPosition
						
						ballRb = ball:GetComponentRigidBody()
						ballRb:ClearForce()
						ballRb:ClearVelocity()
						ballRb.IsGravity = true
						
						ballCs = ball:GetComponentCollisionShape()
						ballCs.IsTrigger = false
						
					elseif tag == "Button3" then
						
						ball = Scene.GetGameObjectWithTag("Ball", 2)
						spawnPoint = Scene.GetGameObjectWithTag("SpawnPoint", 2)
						
						ballTransform = ball:GetComponentTransform()
						spawnPointTransform = spawnPoint:GetComponentTransform()
						
						ballTransform.WorldPosition = spawnPointTransform.WorldPosition
						
						ballRb = ball:GetComponentRigidBody()
						ballRb:ClearForce()
						ballRb:ClearVelocity()
						ballRb.IsGravity = true
						
						ballCs = ball:GetComponentCollisionShape()
						ballCs.IsTrigger = false
		
		
					end
				end	
			end
		end	
	end
end

OnTriggerEnter = function(gameObject, other)
--do your code here

end

OnTriggerExit = function(gameObject, other)
--do your code here

end

OnCollisionEnter = function(gameObject, other)
--do your code here

end

OnCollisionExit = function(gameObject, other)
--do your code here

end


