#include <eve/scripting/python_bridge.hpp>
#include <eve/scripting/script_host.hpp>
#include <gtest/gtest.h>

TEST(ScriptingTest, RegistersHostsAndDispatchesEvents) {
    eve::scripting::ScriptEngine engine;
    engine.register_host(std::make_unique<eve::scripting::PythonScriptHost>());

    bool called = false;
    engine.register_callback("interaction_complete", [&called]() { called = true; });
    EXPECT_TRUE(engine.dispatch_event("interaction_complete"));
    EXPECT_TRUE(called);
}

TEST(ScriptingTest, LoadsVisualGraph) {
    eve::scripting::ScriptEngine engine;
    eve::scripting::VisualScriptGraph graph;
    graph.id = "greet";
    graph.nodes.push_back({"node_1", "OnInteraction", {}, {}, {"emit"}});
    EXPECT_TRUE(engine.load_visual_graph(graph));
}
