Start = function(gameObject)

end

Update = function(gameObject, deltaTime)

end

OnTriggerEnter = function(gameObject, other)

	if other.tag == "Player" then

		Log.DebugLog("Load Next Level")

		buildIndex = SceneManager.GetCurrentSceneId()
		SceneManager.LoadSceneWithId (buildIndex + 1)
		
	end

end

OnTriggerExit = function(gameObject, other)

end

OnCollisionEnter = function(gameObject, other)

end

OnCollisionExit = function(gameObject, other)

end
