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
				
					if tag == "CubeObject" then
		
						rayGoRb = rayGo:GetComponentRigidBody()
						
		
						if rayGoRb.IsGravity == true then
				
							rayGoRb.IsGravity = false
							rayGoRb:ClearForce()
							rayGoRb:ClearVelocity()
						end
						
					end
				end	
			end
		end	
	end
	
	
	goTag = "CubeObject"
	
	if Scene.GetGameObjectWithTagCount(goTag) >= 2 then
	
		go1 = Scene.GetGameObjectWithTag(goTag, 0)
		
		if go1 ~= nil then
		
			go1Rb = go1:GetComponentRigidBody()	
				
			if go1Rb.IsGravity == false then
					
				goT = go1:GetComponentTransform()
				origin = t.WorldPosition + (t:GetWorldForward() * Vector3(8, 8, 8))
				goT.WorldPosition = origin
				goT.WorldRotation = t.WorldRotation
					
			end
		
		end
		
		go2 = Scene.GetGameObjectWithTag(goTag, 1)
		
		if go2 ~= nil then
			go2Rb = go2:GetComponentRigidBody()
			
					
			if go2Rb.IsGravity == false then
					
				goT = go2:GetComponentTransform()
				origin = t.WorldPosition + (t:GetWorldForward() * Vector3(8, 8, 8))
				goT.WorldPosition = origin
				goT.WorldRotation = t.WorldRotation
					
			end
		
		end
			
		
		
		
		if Input.IsKeyboardInputPressed("R") then
		
		
			if go1 ~= nil then
				if go1Rb.IsGravity == false then
				
					go1Rb.IsGravity = true;
					go1Rb:ClearForce()
					go1Rb:ClearVelocity()
					
				end
			end
		
			if go2 ~= nil then
				if go2Rb.IsGravity == false then
				
					go2Rb.IsGravity = true;
					go2Rb:ClearForce()
					go2Rb:ClearVelocity()
					
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


