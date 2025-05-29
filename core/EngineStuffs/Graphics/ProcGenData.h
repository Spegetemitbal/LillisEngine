//
// Created by Somed on 5/28/2025.
//

#ifndef PROCGENDATA_H
#define PROCGENDATA_H
#include <iostream>
#include <ostream>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Color.h"

enum RenderPrimitive
{
    PRIMITIVE_POINT = 0,
    PRIMITIVE_LINE,
    PRIMITIVE_TRIANGLE,
    PRIMITIVE_QUAD
};

struct ProcGenData
{
    std::vector<glm::vec2> vertices = std::vector<glm::vec2>();
    std::vector<Color> colors = std::vector<Color>();
};

struct ChunkData
{
    ChunkData(size_t num_vertices, size_t index)
    {
        this->num_vertices = num_vertices;
        this->index = index;
    }
    size_t num_vertices = 0, index = 0;
};

class ProcGenBatch
{
public:

    ProcGenBatch() = default;
    ~ProcGenBatch() = default;
    ProcGenBatch(RenderPrimitive primitive)
    {
        batchprim = primitive;
    }

    //Returns position in batch.
    size_t InsertData(ProcGenData data)
    {
        if (data.vertices.size() != data.colors.size() || data.vertices.empty())
        {
            std::cout << "Cannot insert nonexistent data" << std::endl;
            return 0;
        }

        positions.emplace_back(data.vertices.size(), vertices.size());
        for (int i = 0; i < data.vertices.size(); i++)
        {
            vertices.push_back(data.vertices[i].x);
            vertices.push_back(data.vertices[i].y);
            colors.push_back(data.colors[i].getRf());
            colors.push_back(data.colors[i].getGf());
            colors.push_back(data.colors[i].getBf());
            colors.push_back(data.colors[i].getAf());
        }
        toUpdate = true;
        return positions.size() - 1;
    }
    void UpdateData(size_t position, ProcGenData data)
    {
        if (position >= positions.size())
        {
            std::cout << "Data does not exist" << std::endl;
            return;
        }
        if (data.vertices.size() != data.colors.size() || data.vertices.empty())
        {
            std::cout << "Cannot accept improper data" << std::endl;
            return;
        }
        if (data.vertices.size() != positions[position].num_vertices)
        {
            std::cout << "Data does not match position" << std::endl;
            return;
        }
        int j = 0;
        for (size_t i = 0; i < positions[position].num_vertices; i++)
        {
            vertices[positions[position].index + j] = data.vertices[i].x;
            j++;
            vertices[positions[position].index + j] = data.vertices[i].y;
            j++;
        }
        j = 0;
        for (size_t i = 0; i < positions[position].num_vertices; i++)
        {
            colors[positions[position].index + j] = data.colors[i].getRf();
            j++;
            colors[positions[position].index + j] = data.colors[i].getGf();
            j++;
            colors[positions[position].index + j] = data.colors[i].getBf();
            j++;
            colors[positions[position].index + j] = data.colors[i].getAf();
            j++;
        }
        toUpdate = true;
    }

private:

    friend class ProcGen;

    RenderPrimitive batchprim = PRIMITIVE_POINT;

    bool toUpdate = false;

    //Position, then number of points, then type.
    std::vector<ChunkData> positions = std::vector<ChunkData>();
    std::vector<float> vertices = std::vector<float>();
    std::vector<float> colors = std::vector<float>();
};

#endif //PROCGENDATA_H
