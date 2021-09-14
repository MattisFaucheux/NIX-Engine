--declare variables here

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)
--do your code here

	txt = gameObject:GetComponentText()
	transform = gameObject:GetComponentTransform()
	
	transform.LocalPosition = Vector3(transform.LocalPosition.X + deltaTime, transform.LocalPosition.Y, transform.LocalPosition.Z)
	
	if txt ~= nil then
		value = math.floor(transform.LocalPosition.X)
		
		if value >= 1000 then
			txt.Text = "Timer: " .. value
		elseif value >= 100 then
			txt.Text = "Timer: " .. value .. " "
		elseif value >= 10 then
			txt.Text = "Timer: " .. value .. "  "
		elseif value < 10 then
			txt.Text = "Timer: " .. value .. "   "
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


