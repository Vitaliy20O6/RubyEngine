#pragma once

#include "VertexBuffer.hpp"

namespace RubyEngine 
{

    class VertexArray 
    {
    private:
        unsigned int m_id = 0;
        unsigned int m_elements_count = 0;

    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertex_buffer) noexcept;
        VertexArray(VertexArray&& vertex_buffer) noexcept;

        void add_buffer(const VertexBuffer& vertex_buffer);
        void bind() const;
        static void unbind();    
    };

}