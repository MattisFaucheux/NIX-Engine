lightTag = "LightObjective"
lightNum = 1
lightIndex = 0

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)
--do your code here

end

OnTriggerEnter = function(gameObject, other)

	if other.tag == "Ball" then
	
		otherRb = other:GetComponentRigidBody()
		otherRb:ClearForce()
		otherRb:ClearVelocity()

		if Scene.GetGameObjectWithTagCount(lightTag) >= lightNum then
	
		lightGo = Scene.GetGameObjectWithTag(lightTag, lightIndex)
		
			if lightGo ~= nil then
			
				lightComp = lightGo:GetComponentLight()
				lightComp:SetDiffuse(Vector3(0, 1, 0))
				lightComp:SetSpecular(Vector3(0, 1, 0))
			
			end
		end
	end

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


