--declare variables here

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)

	transform = gameObject:GetComponentTransform()
	
	if transform ~= nil then
		
		msg = "GameObject Pos Y: " .. (math.floor( transform.LocalPosition.Y *10) /10)
		Log.DebugLog(msg)
	
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


