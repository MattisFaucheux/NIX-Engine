--declare variables here

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)
--do your code here

	button = gameObject:GetComponentImage()
	
	if button:IsClicked() == true then
	
		buildIndex = SceneManager.GetCurrentSceneId()
		SceneManager.LoadSceneWithId (buildIndex +1)
	
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


