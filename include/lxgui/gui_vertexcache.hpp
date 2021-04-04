#ifndef LXGUI_GUI_VERTEXCACHE_HPP
#define LXGUI_GUI_VERTEXCACHE_HPP

#include <lxgui/lxgui.hpp>
#include <lxgui/utils.hpp>
#include "lxgui/gui_vertex.hpp"

#include <vector>
#include <memory>

namespace lxgui {
namespace gui
{
    class material;

    /// An object representing cached vertex data on the GPU
    /** A vertex cache stores vertices and indices that can be used to draw
    *   any shape on the screen. Each group of 3 vertices forms a triangle.
    *   Vertices in the cache can be combined in any order (including by
    *   reusing the same vertex twice) using an index array. For example,
    *   with an index array `ids` and a vertex array `v`, the first triangle
    *   will be drawn using `v[ids[0]]`, `v[ids[1]]`, and `v[ids[2]]`.
    *   Hence, a quad is most efficiently drawn by a `v` array of length
    *   4, containing the vertex at each corner, and an `ids` array of
    *   length 6 containing the indices `[0,1,2,2,3,0]`.
    *
    *   Using this class enables more efficient rendering of large groups
    *   of sprites or quads that share the same material (texture). This
    *   is especially true if the data to render does not change often.
    *
    *   A vertex cache can be rendered with gui::renderer_impl::render_vertex_cache().
    *  .
    *   \note This is an abstract class that must be inherited
    *         from and created by the corresponding gui::renderer_impl.
    */
    class vertex_cache
    {
    public :

        /// Constructor.
        /** \param pMaterial  The material to use to render the vertices
        *   \details A default constructed vertex cache holds no data. Use update()
        *            to store vertices to be rendered.
        */
        explicit vertex_cache(std::shared_ptr<material> pMaterial);

        /// Destructor.
        virtual ~vertex_cache() = default;

        /// Update the data and indices stored in the cache.
        /** \param lVertexData    The vertices to cache
        *   \param lVertexIndices The indices to use for drawing triangles
        *   \note This is equivalent to (but more efficient than) calling
        *         update_data() followed by updated_indices().
        */
        virtual void update(const std::vector<vertex>& lVertexData, const std::vector<uint>& lVertexIndices) = 0;

        /// Update the data stored in the cache, reusing existing indices.
        /** \param lVertexData The vertices to cache
        */
        virtual void update_data(const std::vector<vertex>& lVertexData) = 0;

        /// Update the indices stored in the cache, reusing existing data.
        /** \param lVertexIndices The indices to use for drawing triangles
        */
        virtual void update_indices(const std::vector<uint>& lVertexIndices) = 0;

        /// Returns the material used by this vertex cache.
        /** \return The material used by this vertex cache
        */
        std::shared_ptr<material> get_material() const;

    protected :

        std::shared_ptr<material> pMaterial_;
    };
}
}

#endif