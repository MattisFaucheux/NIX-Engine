totaltime = 0
nbFrame = 0
actualFPS = 0

Start = function(gameObject)
--do your code here
	
end

Update = function(gameObject, deltaTime)
--do your code here
	totaltime = totaltime + deltaTime
	nbFrame = nbFrame + 1
	
	if totaltime > 1 then
	
		actualFPS = math.floor(nbFrame/totaltime)
	
		totaltime = 0
		nbFrame = 0
		
	elseif 	actualFPS <= 0 then
	
		actualFPS = math.floor(1/deltaTime)
	
	end
	

	txt = gameObject:GetComponentText()

	
	if txt ~= nil then
		value = actualFPS
		
		if value >= 1000 then
			txt.Text = "FPS: " .. value .. "  "
		elseif value >= 100 then
			txt.Text = "FPS: " .. value .. "   "
		elseif value >= 10 then
			txt.Text = "FPS: " .. value .. "    "
		elseif value < 10 then
			txt.Text = "FPS: " .. value .. "     "
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


