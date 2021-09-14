rotationSpeed = 70

Start = function(gameObject)

end

Update = function(gameObject, deltaTime)

	t = gameObject:GetComponentTransform()
	
	v = Vector3(0, 1, 0)
	t:RotateLocal(Quaternion.New(rotationSpeed * deltaTime, v))

end

OnTriggerEnter = function(gameObject, other)

end

OnTriggerExit = function(gameObject, other)

end

OnCollisionEnter = function(gameObject, other)

end

OnCollisionExit = function(gameObject, other)

end


