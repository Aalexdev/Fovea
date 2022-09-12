#pragma once

namespace Fovea{
	enum class PhysicalDeviceFeature{
		ROBUST_BUFFER_ACCES,
		FULL_DRAW_INDEX_UINT_32,
		IMAGE_CUBE_ARRAY,
		INDEPENDENT_BLEND,
		GEOMETRY_SHADER,
		TESSELLATION_SHADER,
		SAMPLE_RATE_SHADING,
		DUAL_SRC_BLEND,
		LOGIC_OP,
		MULTI_DRAW_INDIRECT,
		DRAW_INDIRECT_FISRT_INSTANCE,
		DEPTH_CLAMP,
		DEPTH_BIAS_CLAMP,
		FILL_MODE_NON_SOLID,
		DEPTH_BOUNDS,
		WIDE_LINES,
		LARGE_POINTS,
		ALPHA_TO_ONE,
		MULTI_VIEWPORT,
		SAMPLER_ANISOTROPY,
		TEXTURE_COMPRESSION_ETC2,
		TEXTURE_COMPRESSION_ASTC_LDR,
		TEXTURE_COMPRESSION_BC,
		OCCLUSION_QUERY_PRECISE,
		PIPELINE_STATISTICS_QUERY,
		VERTEX_PIPELINE_STORES_AND_ATOMICS,
		FRAGMENT_STORES_AND_ATOMICS,
		SHADER_TESSELLATION_AND_GEOMETRY_POINTS,
		SHADER_IMAGE_GATHER_EXTENDED,
		SHADER_STORAGE_IMAGE_EXTENDED_FORMAT,
		SHADER_STORAGE_IMAGE_MULTISAMPLE,
		SHADER_STORAGE_IMAGE_READ_WITHOUT_ANY_FORMAT,
		SHADER_STORAGE_IMAGE_WRITE_WITHOUT_ANY_FORMAT,
		SHADER_UNIFORM_BUFFER_ARRAY_DYNAMIC_INDEXING,
		SHADER_SAMPLED_IMAGE_ARRAY_DYNAMIC_INDEXING,
		SHADER_STORAGE_BUFFER_ARRAY_DUNAMIC_INDEXING,
		SHADER_STORAGE_IMAGE_ARRAY_DYNAMIC_INDEXING,
		SHADER_CLIP_DISTANCE,
		SHADER_CULL_DISTANCE,
		SHADER_FLOAT_64,
		SHADER_INT_64,
		SHADER_INT_16,
		SHADER_RESOURCE_RESIDENCY,
		SHADER_RESOURCE_MIN_LOD,
		SPARSE_BINDING,
		SPARSE_RESIDENCY_BUFFER,
		SPARSE_RESIDENCY_IMAGE_2D,
		SPARSE_RESIDENCY_IMAGE_3D,
		SPARSE_RESIDENCY_2_SAMPLES,
		SPARSE_RESIDENCY_4_SAMPLES,
		SPARSE_RESIDENCY_8_SAMPLED,
		SPARSE_RESIDENCY_16_SAMPLES,
		SPARSE_RESODENCY_ALIASED,
		VARIABLE_MUTISAMPLE_RATE,
		INHERIED_QUERIES,
		FEATURES_COUNT,
		NONE = -1
	};

	enum QueueFamily{
		FAMILY_GRAPHIC,
		FAMILY_PRESENT,
		FAMILY_COMPUTE,
		FAMILY_TRANSFER,
		FAMILY_PROTECTED,
		FAMILY_SPARSE_BINDING,
		FAMILY_COUNT,
		FAMILY_NONE = -1,
	};

	enum PipelineStage{
		PIPELINE_STAGE_TOP_OF_PIPE,
		PIPELINE_STAGE_DRAW_INDIRECT,
		PIPELINE_STAGE_VERTEX_INPUT,
		PIPELINE_STAGE_VERTEX_SHADER,
		PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER,
		PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER,
		PIPELINE_STAGE_GEOMETRY_SHADER,
		PIPELINE_STAGE_FRAGMENT_SHADER,
		PIPELINE_STAGE_EARLY_FRAGMENT_TESTS,
		PIPELINE_STAGE_LATE_FRAGMENT_TESTS,
		PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT,
		PIPELINE_STAGE_COMPUTE_SHADER,
		PIPELINE_STAGE_TRANSFER,
		PIPELINE_STAGE_BOTTOM_OF_PIPE,
		PIPELINE_STAGE_HOST,
		PIPELINE_STAGE_ALL_GRAPHICS,
		PIPELINE_STAGE_ALL_COMMANDS,
		PIPELINE_STAGE_TRANSFORM_FEEDBACK,
		PIPELINE_STAGE_CONDITIONAL_RENDERING,
		PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD,
		PIPELINE_STAGE_RAY_TRACING_SHADER,
		PIPELINE_STAGE_TASK_SHADER,
		PIPELINE_STAGE_MESH_SHADER,
		PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS,
		PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT,
		PIPELINE_STAGE_COMMAND_PREPROCESS,
		PIPELINE_STAGE_COUNT,
		PIPELINE_STAGE_NONE = -1,
	};

	enum ShaderStage{
		SHADER_STAGE_VERTEX,
		SHADER_STAGE_TESSELLATION_CONTROL,
		SHADER_STAGE_TESSELLATION_EVALUATION,
		SHADER_STAGE_GEOMETRY,
		SHADER_STAGE_FRAGMENT,
		SHADER_STAGE_COMPUTE,
		SHADER_STAGE_ALL_GRAPHICS,
		SHADER_STAGE_ALL,
		SHADER_STAGE_RAYGEN,
		SHADER_STAGE_ANY_HIT,
		SHADER_STAGE_CLOSEST_HIT,
		SHADER_STAGE_MISS,
		SHADER_STAGE_INTERSECTION,
		SHADER_STAGE_CALLABLE,
		SHADER_STAGE_TASK,
		SHADER_STAGE_MESH,
		SHADER_STAGE_COUNT,
	};

	enum class VulkanVersion{
		VULKAN_1_0,
		VULKAN_1_1,
		VULKAN_1_2
	};

	enum class SwapChainRefreshMode{
		MAILBOX_MODE,
		IMMEDIATE_MODE,
		FIFO_MODE,
		VSync = FIFO_MODE,
	};
}