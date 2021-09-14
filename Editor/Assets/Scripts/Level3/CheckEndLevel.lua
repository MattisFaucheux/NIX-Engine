lightTag = "LightObjective"
lightNum = 3

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)
--do your code here

	if Scene.GetGameObjectWithTagCount(lightTag) >= lightNum then
		
		lightGo1 = Scene.GetGameObjectWithTag(lightTag, 0)	
		lightComp1 = lightGo1:GetComponentLight()
		l1 = lightComp1.Diffuse.Y

		
		lightGo2 = Scene.GetGameObjectWithTag(lightTag, 1)	
		lightComp2 = lightGo2:GetComponentLight()
		l2 = lightComp2.Diffuse.Y

		
		lightGo3 = Scene.GetGameObjectWithTag(lightTag, 2)
		lightComp3 = lightGo3:GetComponentLight()
		l3 = lightComp3.Diffuse.Y
		
		if l1 == 1 and l2 == 1 and l3 == 1 then
			
			transform = gameObject:GetComponentTransform()
			transform.WorldPosition = Vector3(280, -25, 178)
		
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


