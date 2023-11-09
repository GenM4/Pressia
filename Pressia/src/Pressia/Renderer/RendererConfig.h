#pragma once

//	2D Renderer Config	////////////////////////////////////////////////////////////////////////////////////////////

//	Engine config flags 
#define PS_RECORD_RENDERER_STATS	1		//Record renderer statistics	1 - Enable, 0 - Disable
#define PS_OPTIMIZE_SHADER_COMPILE	1		//Optimizes shaderc compilation 1 - Enable, 0 - Disable

//	GPU-Based Config (will be set upon querying hardware)
#define PS_MAX_BATCH_DRAW			10000	//Renderer batching				# of quads per batched draw call
#define PS_MAX_TEXTURE_SLOTS		31		//Max texture slots				# of slots - 1 (zero-indexed)
#define PS_MAX_FRAMEBUFFER_SIZE		8192	//Max Framebuffer size			# in pixels





//	2D Renderer Constants

#define PS_WHITE_TEXTURE_SLOT		0
#define PS_WHITE_TEXTURE_DATA		0xffffffff