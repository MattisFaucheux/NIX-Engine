--declare variables here

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)
--do your code here

end

OnTriggerEnter = function(gameObject, other)

	if other.tag == "Player" then

		Log.DebugLog("Reload Level")

		otherTransform = other:GetComponentTransform()
		otherTransform.WorldPosition = Vector3(20, 4, 40)
		otherTransform.WorldRotation = Quaternion.New(-90, Vector3(0, 1, 0))
		--buildIndex = SceneManager.GetCurrentSceneId()
		--SceneManager.LoadSceneWithId (buildIndex)
		
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


