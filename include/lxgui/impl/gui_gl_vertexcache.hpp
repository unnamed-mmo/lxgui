#ifndef LXGUI_GUI_GL_VERTEXCACHE_HPP
#define LXGUI_GUI_GL_VERTEXCACHE_HPP

#include "lxgui/impl/gui_gl_material.hpp"

#include <lxgui/gui_vertexcache.hpp>
#include <lxgui/utils.hpp>

#include <memory>

namespace lxgui {
namespace gui {
namespace gl
{
    /// A place to render things (the screen, a texture, ...)
    class vertex_cache : public gui::vertex_cache
    {
    public :

        /// Constructor.
        /** \param uiWidth  The width of the vertex_cache
        *   \param uiHeight The height of the vertex_cache
        */
        vertex_cache(std::shared_ptr<gui::material> pMaterial);

        /// Destructor.
        ~vertex_cache() override;

        /// Update the data and indices stored in the cache.
        /** \param lVertexData    The vertices to cache
        *   \param lVertexIndices The indices to use for drawing triangles
        *   \note This is equivalent to (but more efficient than) calling
        *         update_data() followed by updated_indices().
        */
        void update(const std::vector<vertex>& lVertexData, const std::vector<uint>& lVertexIndices) override;

        /// Update the data stored in the cache, reusing existing indices.
        /** \param lVertexData The vertices to cache
        */
        void update_data(const std::vector<vertex>& lVertexData) override;

        /// Update the indices stored in the cache, reusing existing data.
        /** \param lVertexIndices The indices to use for drawing triangles
        */
        void update_indices(const std::vector<uint>& lVertexIndices) override;

    private :

        uint uiVertexArray_ = (uint)-1;
        uint uiVertexBuffer_ = (uint)-1;
        uint uiIndexBuffer_ = (uint)-1;
    };
}
}
}

#endif
