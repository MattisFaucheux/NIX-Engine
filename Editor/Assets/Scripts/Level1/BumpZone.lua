jumpForce = 25000

Start = function(gameObject)

end

Update = function(gameObject, deltaTime)

end

OnTriggerEnter = function(gameObject, other)

	tag = other.tag


	if tag == "Player" then
		
		t = other:GetComponentTransform()
		rb = other:GetComponentRigidBody()

		jump = Vector3(0, jumpForce, 0)
		forceMode = "Force"
			
		rb:AddForce(jump, forceMode)
	end
	

end

OnTriggerExit = function(gameObject, other)

end

OnCollisionEnter = function(gameObject, other)

end

OnCollisionExit = function(gameObject, other)

end
