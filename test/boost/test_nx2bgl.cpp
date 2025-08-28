#include <doctest/doctest.h>

#include <boost/graph/adjacency_list.hpp>
#include <py2cpp/nx2bgl.hpp>

TEST_CASE("Test GrAdaptor") {
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
    auto G = py::GrAdaptor<Graph>(Graph(3));
    CHECK(G.number_of_nodes() == 3);
    CHECK(G.number_of_edges() == 0);

    auto [e, inserted] = G.add_edge(0, 1);
    CHECK(inserted);
    CHECK(G.number_of_edges() == 1);

    auto s = G.source(e);
    auto t = G.target(e);
    CHECK(s == 0);
    CHECK(t == 1);

    auto [s2, t2] = G.end_points(e);
    CHECK(s2 == 0);
    CHECK(t2 == 1);
}

TEST_CASE("Test VertexView") {
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
    auto G = py::GrAdaptor<Graph>(Graph(4));
    G.add_edge(0, 1);
    G.add_edge(1, 2);
    G.add_edge(2, 3);

    auto count = 0;
    for (auto v : G) {
        (void)v;
        count++;
    }
    CHECK(count == 4);
}

TEST_CASE("Test EdgeView") {
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
    auto G = py::GrAdaptor<Graph>(Graph(3));
    G.add_edge(0, 1);
    G.add_edge(1, 2);

    auto count = 0;
    for (auto e : G.edges()) {
        (void)e;
        count++;
    }
    CHECK(count == 2);
}

TEST_CASE("Test AtlasView") {
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>;
    auto G = py::GrAdaptor<Graph>(Graph(4));
    G.add_edge(0, 1);
    G.add_edge(0, 2);
    G.add_edge(0, 3);

    auto count = 0;
    for (auto e : G.neighbors(0)) {
        (void)e;
        count++;
    }
    CHECK(count == 3);
}
