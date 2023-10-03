#pragma once

// 2D Renderer Config
#define PS_MAX_BATCH_DRAW			10000;	//Renderer batching				# of quads per batched draw call
#define PS_MAX_TEXTURE_SLOTS		31;		//Max texture slots				# of slots - 1 (zero-indexed)
#define PS_RECORD_RENDERER_STATS	1;		//Record renderer statistics	1 - Enable, 0 - Disable