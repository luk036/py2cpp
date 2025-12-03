#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_utility.hpp>
#include <type_traits>

namespace py {

    /**
     * @brief Vertex view for Boost Graph Library graphs
     *
     * Provides iterable access to vertices in a BGL graph. This class wraps a graph
     * and provides iterator interfaces for traversing vertices.
     *
     * @tparam Graph The Boost Graph Library graph type
     */
    template <typename Graph> class VertexView : public Graph {
      public:
        /**
         * @brief Construct a new Vertex View object
         *
         * Creates a vertex view from an existing graph by moving it into this wrapper.
         *
         * @param[in] gra The graph to wrap (moved into this VertexView)
         */
        explicit VertexView(Graph &&gra) noexcept : Graph{std::forward<Graph>(gra)} {}

        /**
         * @brief Get iterator to the beginning of vertices
         *
         * Returns an iterator to the first vertex in the graph.
         *
         * @return Iterator to the first vertex
         */
        [[nodiscard]] auto begin() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_iter;
            return boost::vertices(*this).first;
        }

        /**
         * @brief Get iterator to the end of vertices
         *
         * Returns an iterator past the last vertex in the graph.
         *
         * @return Iterator past the last vertex
         */
        [[nodiscard]] auto end() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_end;
            return boost::vertices(*this).second;
        }

        /**
         * @brief Get const iterator to the beginning of vertices
         *
         * Returns a const iterator to the first vertex in the graph.
         *
         * @return Const iterator to the first vertex
         */
        [[nodiscard]] auto cbegin() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_iter;
            return boost::vertices(*this).first;
        }

        /**
         * @brief Get const iterator to the end of vertices
         *
         * Returns a const iterator past the last vertex in the graph.
         *
         * @return Const iterator past the last vertex
         */
        [[nodiscard]] auto cend() const {
            // auto [v_iter, v_end] = boost::vertices(*this);
            // return v_end;
            return boost::vertices(*this).second;
        }
    };

    /**
     * @brief Edge view for Boost Graph Library graphs
     *
     * Provides iterable access to edges in a BGL graph. This class holds a reference
     * to a graph and provides iterator interfaces for traversing edges.
     *
     * @tparam Graph The Boost Graph Library graph type
     */
    template <typename Graph> class EdgeView {
      private:
        const Graph &gra;

      public:
        /**
         * @brief Construct a new Edge View object
         *
         * Creates an edge view from a reference to an existing graph.
         *
         * @param[in] gra Reference to the graph to view edges from
         */
        explicit EdgeView(const Graph &gra) : gra{gra} {}

        /**
         * @brief Get iterator to the beginning of edges
         *
         * Returns an iterator to the first edge in the graph.
         *
         * @return Iterator to the first edge
         */
        [[nodiscard]] auto begin() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_iter;
            return boost::edges(this->gra).first;
        }

        /**
         * @brief Get iterator to the end of edges
         *
         * Returns an iterator past the last edge in the graph.
         *
         * @return Iterator past the last edge
         */
        [[nodiscard]] auto end() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_end;
            return boost::edges(this->gra).second;
        }

        /**
         * @brief Get const iterator to the beginning of edges
         *
         * Returns a const iterator to the first edge in the graph.
         *
         * @return Const iterator to the first edge
         */
        [[nodiscard]] auto cbegin() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_iter;
            return boost::edges(this->gra).first;
        }

        /**
         * @brief Get const iterator to the end of edges
         *
         * Returns a const iterator past the last edge in the graph.
         *
         * @return Const iterator past the last edge
         */
        [[nodiscard]] auto cend() const {
            // auto [e_iter, e_end] = boost::edges(_gra);
            // return e_end;
            return boost::edges(this->gra).second;
        }
    };

    /**
     * @brief Atlas view for vertex adjacency in Boost Graph Library graphs
     *
     * Provides iterable access to edges adjacent to a specific vertex in a BGL graph.
     * This class holds a vertex and a reference to a graph and provides iterator
     * interfaces for traversing edges connected to that vertex.
     *
     * @tparam Vertex The vertex descriptor type
     * @tparam Graph The Boost Graph Library graph type
     */
    template <typename Vertex, typename Graph> class AtlasView {
      private:
        Vertex _v;
        const Graph &gra;

      public:
        /**
         * @brief Construct a new Atlas View object
         *
         * Creates an atlas view for a specific vertex in a graph.
         *
         * @param[in] v The vertex to view adjacency from
         * @param[in] gra Reference to the graph containing the vertex
         */
        AtlasView(Vertex v, const Graph &gra) : _v{v}, gra{gra} {}

        /**
         * @brief Get iterator to the beginning of adjacent edges
         *
         * Returns an iterator to the first edge adjacent to the vertex.
         *
         * @return Iterator to the first adjacent edge
         */
        auto begin() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_iter;
            return boost::out_edges(this->_v, this->gra).first;
        }

        /**
         * @brief Get iterator to the end of adjacent edges
         *
         * Returns an iterator past the last edge adjacent to the vertex.
         *
         * @return Iterator past the last adjacent edge
         */
        auto end() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_end;
            return boost::out_edges(this->_v, this->gra).second;
        }

        /**
         * @brief Get const iterator to the beginning of adjacent edges
         *
         * Returns a const iterator to the first edge adjacent to the vertex.
         *
         * @return Const iterator to the first adjacent edge
         */
        auto cbegin() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_iter;
            return boost::out_edges(this->_v, this->gra).first;
        }

        /**
         * @brief Get const iterator to the end of adjacent edges
         *
         * Returns a const iterator past the last edge adjacent to the vertex.
         *
         * @return Const iterator past the last adjacent edge
         */
        auto cend() const {
            // auto [e_iter, e_end] = boost::out_edges(_v, _gra);
            // return e_end;
            return boost::out_edges(this->_v, this->gra).second;
        }
    };

    /**
     * @brief Graph adapter for Boost Graph Library integration
     *
     * Provides a unified interface for working with BGL graphs, combining vertex
     * and edge view functionality with additional graph operations.
     *
     * @tparam _Graph The Boost Graph Library graph type
     */
    template <typename _Graph> class GrAdaptor : public VertexView<_Graph> {
      public:
        using Vertex = typename boost::graph_traits<_Graph>::vertex_descriptor;
        using node_t = Vertex;
        using edge_t = typename boost::graph_traits<_Graph>::edge_descriptor;

        // using edge_wt_t = decltype( boost::get(boost::edge_weight,
        // std::declval<_Graph>()) );

        /**
         * @brief Default constructor (deleted)
         *
         * Graph adaptor cannot be default constructed.
         */
        GrAdaptor() = delete;

        /**
         * @brief Construct a new graph adaptor object
         *
         * Creates a graph adaptor by moving an existing graph into this wrapper.
         *
         * @param[in] gra The graph to wrap (moved into this GrAdaptor)
         */
        explicit GrAdaptor(_Graph &&gra) noexcept : VertexView<_Graph>{std::forward<_Graph>(gra)} {}

        // GrAdaptor(const GrAdaptor&) = delete;            // don't copy
        // GrAdaptor& operator=(const GrAdaptor&) = delete; // don't assign
        // GrAdaptor(GrAdaptor&&) noexcept = default;                // don't copy

        /**
         * @brief Get the number of vertices in the graph
         *
         * @return Number of vertices
         */
        [[nodiscard]] auto number_of_nodes() const { return boost::num_vertices(*this); }

        /**
         * @brief Get the number of edges in the graph
         *
         * @return Number of edges
         */
        [[nodiscard]] auto number_of_edges() const { return boost::num_edges(*this); }

        /**
         * @brief Get an edge view for the graph
         *
         * Returns an iterable view of all edges in the graph.
         *
         * @return EdgeView<_Graph> Iterable edge view
         */
        [[nodiscard]] auto edges() const -> EdgeView<_Graph> { return EdgeView<_Graph>(*this); }

        /**
         * @brief Get neighbors of a vertex
         *
         * Returns an iterable view of edges adjacent to the specified vertex.
         *
         * @param[in] v The vertex to get neighbors for
         * @return AtlasView<Vertex, _Graph> Iterable view of adjacent edges
         */
        [[nodiscard]] auto neighbors(Vertex v) const -> AtlasView<Vertex, _Graph> {
            return AtlasView<Vertex, _Graph>(v, *this);
        }

        /**
         * @brief
         *
         * @param[in] u
         * @param[in] v
         */
        auto add_edge(int u, int v) {
            return boost::add_edge(static_cast<Vertex>(u), static_cast<Vertex>(v), *this);
        }

        /**
         * @brief Get the null vertex descriptor
         *
         * Returns the special null vertex value used by the graph.
         *
         * @return Vertex Null vertex descriptor
         */
        static auto null_vertex() -> Vertex { return boost::graph_traits<_Graph>::null_vertex(); }

        /**
         * @brief Get the source vertex of an edge
         *
         * Returns the source (origin) vertex of the specified edge.
         *
         * @tparam Edge The edge descriptor type
         * @param[in] e The edge to get the source of
         * @return Vertex Source vertex of the edge
         */
        template <typename Edge> auto source(const Edge &e) const -> Vertex {
            return boost::source(e, *this);
        }

        /**
         * @brief Get the target vertex of an edge
         *
         * Returns the target (destination) vertex of the specified edge.
         *
         * @tparam Edge The edge descriptor type
         * @param[in] e The edge to get the target of
         * @return Vertex Target vertex of the edge
         */
        template <typename Edge> auto target(const Edge &e) const -> Vertex {
            return boost::target(e, *this);
        }

        /**
         * @brief
         *
         * @tparam Edge
         * @param[in] e
         */
        template <typename Edge> [[nodiscard]] auto end_points(const Edge &e) const {
            auto s = boost::source(e, *this);
            auto t = boost::target(e, *this);
            return std::make_pair(s, t);
        }
    };

}  // namespace py
