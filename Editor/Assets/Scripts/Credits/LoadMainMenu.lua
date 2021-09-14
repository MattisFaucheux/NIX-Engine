--declare variables here

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)

	button = gameObject:GetComponentImage()
	
	if button:IsClicked() == true then
	
		SceneManager.LoadSceneWithId (0)
	
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


