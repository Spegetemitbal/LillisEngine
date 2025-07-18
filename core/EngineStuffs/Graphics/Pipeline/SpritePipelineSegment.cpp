//
// Created by Somed on 6/16/2025.
//

#include "SpritePipelineSegment.h"
#include <glad/gl.h>
#include <glm/ext/matrix_transform.hpp>

#include "Utils/ResourceManager.h"
#include "../BackgroundManager.h"
#include "EngineStuffs/Graphics/Parallax.h"
#include "EngineStuffs/Graphics/RenderOrder.h"

SpritePipelineSegment::SpritePipelineSegment(RenderSettings render_settings, LILLIS::Shader shader) : PipelineSegment(render_settings)
{
    SetShader(shader);
}

void SpritePipelineSegment::InitSegment()
{
    // configure VAO/VBO
    //Maybe swap to triangle strips for efficiency?

    VAOs.push_back(0);
    VBOs.push_back(0);
    VBOs.push_back(0);

    FBOs.push_back(0);
    colorBuffers.push_back(0);
    depthBuffers.push_back(0);

    //Generate frame buffer.
    glGenFramebuffers(1, FBOs.data());
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);

    //8 bit RGBA color buffer
    glGenTextures(1, colorBuffers.data());
    glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (GLsizei)render_settings.resolutionWidthWBuffer(), (GLsizei)render_settings.resolutionHeightWBuffer());
    if (render_settings.pixelPerfect)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    //Make depth buffer.
    glGenTextures(1, depthBuffers.data());
    glBindTexture(GL_TEXTURE_2D, depthBuffers[0]);
    //Maybe use a higher precision buffer if issues are arising.
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, (GLsizei)render_settings.resolutionWidthWBuffer(), (GLsizei)render_settings.resolutionHeightWBuffer());

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBuffers[0], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffers[0],0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCreateVertexArrays(1, VAOs.data());
    glGenBuffers(2, VBOs.data());

    glBindVertexArray(VAOs[0]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    float positions[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    float texCoords[] = {
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    //float rendValue = 0.0f;

    //load positions
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Load texture coordinates.
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0);
    glViewport(0, 0, (GLsizei)render_settings.resolutionWidthWBuffer(), (GLsizei)render_settings.resolutionHeightWBuffer());
}

void SpritePipelineSegment::PreRender()
{
    shader.Use();
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[0]);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    glViewport(0, 0, (GLsizei)render_settings.resolutionWidthWBuffer(), (GLsizei)render_settings.resolutionHeightWBuffer());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


std::vector<ColorBufferWrapper> SpritePipelineSegment::DoStep(std::vector<Sprite *> &sprites, unsigned int lastSprite, std::vector<TileMap> &tile_maps, LILLIS::Camera& camera)
{
    if (deferredRender)
    {
        //Note, if not in parallax, default to top layer.
        unsigned int high = RenderOrder::GetHighestLayer();
        int highestColorBuffer = 0;
        //first is layer, second is buffer.
        std::unordered_map<unsigned int, int> layersToParallax;
        std::vector<ColorBufferWrapper> bufferTime;

        //Get necessary data to render.
        for (unsigned int i = 0; i <= high; i++)
        {
            if (Parallax::isLayerParallax((int)i))
            {
                if (i == Parallax::getCenterLayer())
                {
                    layersToParallax.emplace(i, 0);
                } else
                {
                    highestColorBuffer++;
                    layersToParallax.emplace(i, highestColorBuffer);
                }
            }
        }
        if (highestColorBuffer < 0)
        {
            highestColorBuffer = 0;
        }

        //Ensure num layer stuff is acceptable.
        int maxBuffers;
        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxBuffers);

        //Ensure max backgrounds hasn't been reached.
        if (highestColorBuffer + 1 > maxBuffers)
        {
            std::cout << "Max buffers reached in Sprite Rendering" << std::endl;
            return bufferTime;
        }

        while (colorBuffers.size() < highestColorBuffer + 1)
        {
            colorBuffers.push_back(0);
            //8 bit RGBA color buffer
            glGenTextures(1, &colorBuffers.back());
            glBindTexture(GL_TEXTURE_2D, colorBuffers.back());
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (GLsizei)render_settings.resolutionHeightWBuffer(), (GLsizei)render_settings.resolutionHeightWBuffer());
            if (render_settings.pixelPerfect)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (colorBuffers.size() - 1), colorBuffers.back(), 0);
        }

        int currentBuffer = 0, currentLayer = 0;

        const GLenum defaultAttachment = GL_COLOR_ATTACHMENT0;
        glDrawBuffers(1, &defaultAttachment);


        //Render other stuff.
        for (auto & tMap: tile_maps)
        {
            if (tMap.active)
            {
                if (tMap.GetLayer() > currentLayer)
                {
                    currentLayer = (int)tMap.GetLayer();
                    if (layersToParallax.contains(currentLayer))
                    {
                        if (currentBuffer != layersToParallax[currentLayer])
                        {
                            currentBuffer = layersToParallax[currentLayer];
                            const GLenum attachment = GL_COLOR_ATTACHMENT0 + currentBuffer;
                            glDrawBuffers(1, &attachment);
                        }
                    } else if (currentBuffer < colorBuffers.size() - 1)
                    {
                        glDrawBuffers(1, &defaultAttachment);
                    }
                }

                glm::vec2 renderSize = tMap.getTileSize();
                for (int i = 0; i < tMap.tilesToRender.size(); i++)
                {
                    TileLoc t = tMap.tilesToRender[i];
                    std::string img = tMap.getImageFromIndex(t.tile);
                    unsigned int frm = tMap.getFrameFromIndex(t.tile);
                    if (img.empty())
                    {
                        throw;
                    }
                    Texture2D tex = ResourceManager::GetTexture(img);
                    glm::vec2 parOffset = Parallax::doParallaxOffset((int)tMap.GetLayer(), t.worldPos, camera.position);
                    RenderSprite(tex, t.worldPos + parOffset, t.zVal, (int)frm, camera.projectionMatrix(render_settings.pixelPerfect),
                        renderSize, 0);
                }
            }
        }

        currentBuffer = 0;
        currentLayer = 0;
        glDrawBuffers(1, &defaultAttachment);

        //Render Sprites
        for (int i = 0; i < sprites.size(); i++)
        {
            Sprite* spr = sprites[i];

            if (spr->getLayer() > currentLayer)
            {
                currentLayer = (int)spr->getLayer();
                if (layersToParallax.contains(currentLayer))
                {
                    if (currentBuffer != layersToParallax[currentLayer])
                    {
                        currentBuffer = layersToParallax[currentLayer];
                        const GLenum attachment = GL_COLOR_ATTACHMENT0 + currentBuffer;
                        glDrawBuffers(1, &attachment);
                    }
                } else if (currentBuffer < colorBuffers.size() - 1)
                {
                    glDrawBuffers(1, &defaultAttachment);
                }
            }

            if (spr->getImageName().empty())
            {
                throw;
            }
            Texture2D tex = ResourceManager::GetTexture(spr->getImageName());
            glm::vec2 parOffset = Parallax::doParallaxOffset((int)spr->getLayer(), spr->getRenderLocation(), camera.position);
            RenderSprite(tex, spr->getRenderLocation() + parOffset, spr->getRenderValue(), (int)spr->frame, camera.projectionMatrix(render_settings.pixelPerfect),
                spr->RenderSize() * spr->getRenderScale(), spr->getRenderRotation());
        }

        for (auto it : layersToParallax)
        {
            bufferTime.emplace_back(doPostProcess, it.first, colorBuffers[it.second]);
        }

        glDrawBuffers(1, &defaultAttachment);
        return bufferTime;
    }

    for (auto & tMap: tile_maps)
    {
        if (tMap.active)
        {
            glm::vec2 renderSize = tMap.getTileSize();
            for (int i = 0; i < tMap.tilesToRender.size(); i++)
            {
                TileLoc t = tMap.tilesToRender[i];
                std::string img = tMap.getImageFromIndex(t.tile);
                unsigned int frm = tMap.getFrameFromIndex(t.tile);
                if (img.empty())
                {
                    throw;
                }
                Texture2D tex = ResourceManager::GetTexture(img);
                if (doParallax)
                {
                    glm::vec2 parallaxOffset = Parallax::doParallaxOffset((int)tMap.GetLayer(), t.worldPos, camera.position);
                    RenderSprite(tex, t.worldPos + parallaxOffset, t.zVal, (int)frm, camera.projectionMatrix(render_settings.pixelPerfect),
                    renderSize, 0);
                    continue;
                }
                RenderSprite(tex, t.worldPos, t.zVal, (int)frm, camera.projectionMatrix(render_settings.pixelPerfect),
                    renderSize, 0);
            }
        }
    }

    for (int i = 0; i < sprites.size(); i++)
    {
        Sprite* spr = sprites[i];
        if (spr->getImageName().empty())
        {
            throw;
        }
        Texture2D tex = ResourceManager::GetTexture(spr->getImageName());
        if (doParallax)
        {
            glm::vec2 parOffset = Parallax::doParallaxOffset((int)spr->getLayer(), spr->getRenderLocation(), camera.position);
            RenderSprite(tex, spr->getRenderLocation() + parOffset, spr->getRenderValue(), (int)spr->frame, camera.projectionMatrix(render_settings.pixelPerfect),
                spr->RenderSize() * spr->getRenderScale(), spr->getRenderRotation());
            continue;
        }
        RenderSprite(tex, spr->getRenderLocation(), spr->getRenderValue(), (int)spr->frame, camera.projectionMatrix(render_settings.pixelPerfect),
            spr->RenderSize() * spr->getRenderScale(), spr->getRenderRotation());
    }

    return {ColorBufferWrapper(doPostProcess, 0, colorBuffers[0])};
}

void SpritePipelineSegment::RenderSprite(const Texture2D &texture, glm::vec2 position, float renderVal, int frame, glm::mat4 camera, glm::vec2 size, float rotate, glm::vec3 color)
{
    size *= (float)render_settings.pixelsPerUnit;
    position *= (float)render_settings.pixelsPerUnit;
    // prepare transformations
    glm::mat4 model = glm::mat4(1.0f);
    //glm::vec2 flippedPosition = {position.x, -position.y};
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    glm::vec4 newColor = glm::vec4(color, 1.0f);

    shader.SetMatrix4("model", model);
    shader.SetVector4f("spriteColor", newColor);
    shader.SetFloat("renderValue", renderVal);
    shader.SetInteger("image", 0);
    shader.SetMatrix4("projection", camera);

    glm::vec4 spriteQuad = texture.spriteLocations[frame];
    float texCoords[] =
    {
        spriteQuad.x, spriteQuad.w,
        spriteQuad.y, spriteQuad.z,
        spriteQuad.x, spriteQuad.z,

        spriteQuad.x, spriteQuad.w,
        spriteQuad.y, spriteQuad.w,
        spriteQuad.y, spriteQuad.z
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texCoords), texCoords);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


