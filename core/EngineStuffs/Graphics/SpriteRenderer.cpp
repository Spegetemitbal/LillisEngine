#include "SpriteRenderer.h"
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void SpriteRenderer::initRenderData()
{
    // configure VAO/VBO
    //Maybe swap to triangle strips for efficiency?

    glCreateVertexArrays(1, &quadVAO);
    glGenBuffers(2, VBO);

    glBindVertexArray(quadVAO);
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
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Load texture coordinates.
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::shutdownRenderData()
{
    glDeleteVertexArrays(1, &quadVAO);
}


void SpriteRenderer::DrawSprite(const Texture2D& texture, glm::vec2 position, float renderVal, int frame,
    glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    spriteShader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    //glm::vec2 flippedPosition = {position.x, -position.y};
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, -glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    spriteShader.SetMatrix4("model", model);
    spriteShader.SetVector3f("spriteColor", color);
    spriteShader.SetFloat("renderValue", renderVal);
    spriteShader.SetInteger("image", 0);

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

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texCoords), texCoords);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawUI(const Texture2D &texture, glm::vec2 position, int frame, glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    uiShader.Use();

    uiShader.SetVector3f("spriteColor", color);
    uiShader.SetInteger("image", 0);

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

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texCoords), texCoords);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
