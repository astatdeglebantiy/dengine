#include "renderer.h"

#include <string.h>

static SDL_GPUShader* load_shader(
    SDL_GPUDevice* gpu,
    const char* path,
    SDL_GPUShaderStage stage
) {
    size_t size = 0;
    void* code = SDL_LoadFile(path, &size);
    if (!code) {
        SDL_Log("Failed to load shader %s: %s", path, SDL_GetError());
        return NULL;
    }

    SDL_GPUShaderCreateInfo info = {
        .code_size = size,
        .code = (const Uint8*)code,
        .entrypoint = "main",
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = stage,
        .num_samplers = 0,
        .num_storage_textures = 0,
        .num_storage_buffers = 0,
        .num_uniform_buffers = 0
    };

    SDL_GPUShader* shader = SDL_CreateGPUShader(gpu, &info);
    SDL_free(code);
    if (!shader) {
        SDL_Log("Failed to create shader %s: %s", path, SDL_GetError());
    }
    return shader;
}

Renderer Renderer_create(Window* win) {
    Renderer r = {0};
    if (!win || !win->gpu_device || !win->sdl_window) {
        SDL_Log("Renderer_create received an invalid window or GPU device.");
        return r;
    }

    SDL_GPUDevice* gpu = win->gpu_device;
    static const Vertex VERTICES[3] = {
        {  0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f },
        { -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f }
    };

    SDL_GPUBufferCreateInfo buf_info = {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof(VERTICES)
    };
    r.vertex_buffer = SDL_CreateGPUBuffer(gpu, &buf_info);
    if (!r.vertex_buffer) {
        SDL_Log("Failed to create vertex buffer: %s", SDL_GetError());
        return r;
    }

    SDL_GPUTransferBufferCreateInfo transfer_info = {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = sizeof(VERTICES)
    };
    SDL_GPUTransferBuffer* transfer_buf = SDL_CreateGPUTransferBuffer(gpu, &transfer_info);
    if (!transfer_buf) {
        SDL_Log("Failed to create transfer buffer: %s", SDL_GetError());
        Renderer_destroy(&r, win);
        return (Renderer){0};
    }

    void* map = SDL_MapGPUTransferBuffer(gpu, transfer_buf, false);
    if (!map) {
        SDL_Log("Failed to map transfer buffer: %s", SDL_GetError());
        SDL_ReleaseGPUTransferBuffer(gpu, transfer_buf);
        Renderer_destroy(&r, win);
        return (Renderer){0};
    }
    memcpy(map, VERTICES, sizeof(VERTICES));
    SDL_UnmapGPUTransferBuffer(gpu, transfer_buf);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(gpu);
    if (!cmd) {
        SDL_Log("Failed to acquire GPU command buffer: %s", SDL_GetError());
        SDL_ReleaseGPUTransferBuffer(gpu, transfer_buf);
        Renderer_destroy(&r, win);
        return (Renderer){0};
    }

    SDL_GPUCopyPass* copy = SDL_BeginGPUCopyPass(cmd);
    if (!copy) {
        SDL_Log("Failed to begin GPU copy pass: %s", SDL_GetError());
        SDL_CancelGPUCommandBuffer(cmd);
        SDL_ReleaseGPUTransferBuffer(gpu, transfer_buf);
        Renderer_destroy(&r, win);
        return (Renderer){0};
    }

    SDL_GPUTransferBufferLocation src = {
        .transfer_buffer = transfer_buf,
        .offset = 0
    };
    SDL_GPUBufferRegion dst = {
        .buffer = r.vertex_buffer,
        .offset = 0,
        .size = sizeof(VERTICES)
    };
    SDL_UploadToGPUBuffer(copy, &src, &dst, false);
    SDL_EndGPUCopyPass(copy);
    if (!SDL_SubmitGPUCommandBuffer(cmd)) {
        SDL_Log("Failed to submit GPU upload: %s", SDL_GetError());
        SDL_ReleaseGPUTransferBuffer(gpu, transfer_buf);
        Renderer_destroy(&r, win);
        return (Renderer){0};
    }
    SDL_ReleaseGPUTransferBuffer(gpu, transfer_buf);

    SDL_GPUShader* vert_shader = load_shader(
        gpu, "shaders/triangle_vert.spv", SDL_GPU_SHADERSTAGE_VERTEX
    );
    SDL_GPUShader* frag_shader = load_shader(
        gpu, "shaders/triangle_frag.spv", SDL_GPU_SHADERSTAGE_FRAGMENT
    );
    if (!vert_shader || !frag_shader) {
        if (vert_shader) SDL_ReleaseGPUShader(gpu, vert_shader);
        if (frag_shader) SDL_ReleaseGPUShader(gpu, frag_shader);
        Renderer_destroy(&r, win);
        return (Renderer){0};
    }

    SDL_GPUVertexAttribute attrs[2] = {
        {
            .location = 0,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = 0
        },
        {
            .location = 1,
            .buffer_slot = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
            .offset = sizeof(float) * 3
        }
    };
    SDL_GPUVertexBufferDescription v_buf_desc = {
        .slot = 0,
        .pitch = sizeof(Vertex),
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX
    };
    SDL_GPUColorTargetDescription color_desc = {
        .format = SDL_GetGPUSwapchainTextureFormat(gpu, win->sdl_window)
    };
    SDL_GPUGraphicsPipelineCreateInfo pipeline_info = {
        .vertex_shader = vert_shader,
        .fragment_shader = frag_shader,
        .vertex_input_state = {
            .vertex_attributes = attrs,
            .num_vertex_attributes = 2,
            .vertex_buffer_descriptions = &v_buf_desc,
            .num_vertex_buffers = 1
        },
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .target_info = {
            .color_target_descriptions = &color_desc,
            .num_color_targets = 1
        }
    };

    r.pipeline = SDL_CreateGPUGraphicsPipeline(gpu, &pipeline_info);
    SDL_ReleaseGPUShader(gpu, vert_shader);
    SDL_ReleaseGPUShader(gpu, frag_shader);

    if (!r.pipeline) {
        SDL_Log("Failed to create graphics pipeline: %s", SDL_GetError());
        Renderer_destroy(&r, win);
        return (Renderer){0};
    }

    return r;
}

void Renderer_draw(Renderer* self, Window* win) {
    if (!self || !self->pipeline || !self->vertex_buffer ||
        !win || !win->gpu_device || !win->sdl_window) {
        return;
    }

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(win->gpu_device);
    if (!cmd) {
        SDL_Log("Failed to acquire GPU command buffer for frame: %s", SDL_GetError());
        return;
    }

    SDL_GPUTexture* swapchain = NULL;
    if (!SDL_AcquireGPUSwapchainTexture(
            cmd, win->sdl_window, &swapchain, NULL, NULL)) {
        SDL_Log("Failed to acquire swapchain texture: %s", SDL_GetError());
        SDL_CancelGPUCommandBuffer(cmd);
        return;
    }

    if (!swapchain) {
        SDL_SubmitGPUCommandBuffer(cmd);
        return;
    }

    SDL_GPUColorTargetInfo target = {
        .texture = swapchain,
        .clear_color = { 0.12f, 0.12f, 0.15f, 1.0f },
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE
    };
    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmd, &target, 1, NULL);
    if (!pass) {
        SDL_Log("Failed to begin GPU render pass: %s", SDL_GetError());
        SDL_CancelGPUCommandBuffer(cmd);
        return;
    }

    SDL_BindGPUGraphicsPipeline(pass, self->pipeline);
    SDL_GPUBufferBinding binding = {
        .buffer = self->vertex_buffer,
        .offset = 0
    };
    SDL_BindGPUVertexBuffers(pass, 0, &binding, 1);
    SDL_DrawGPUPrimitives(pass, 3, 1, 0, 0);
    SDL_EndGPURenderPass(pass);

    if (!SDL_SubmitGPUCommandBuffer(cmd)) {
        SDL_Log("Failed to submit frame to GPU: %s", SDL_GetError());
    }
}

void Renderer_destroy(Renderer* self, Window* win) {
    if (!self || !win || !win->gpu_device) {
        return;
    }
    if (self->pipeline) {
        SDL_ReleaseGPUGraphicsPipeline(win->gpu_device, self->pipeline);
        self->pipeline = NULL;
    }
    if (self->vertex_buffer) {
        SDL_ReleaseGPUBuffer(win->gpu_device, self->vertex_buffer);
        self->vertex_buffer = NULL;
    }
}
