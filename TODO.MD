Fovea:
	tests:
		- instance @done
		- physical device @done
		- logical device @done
		- swapchain
		- render target
		
		pipeline:
			- default graphic pipeline
			- push constant
			- descriptor set
		 
		descriptor_set:
			- data buffer
			- texture buffer

		buffer:
			- static buffer
			- dynamic buffer
	
	pipelines:
		- graphic pipeline @done
		- compute pipeline
