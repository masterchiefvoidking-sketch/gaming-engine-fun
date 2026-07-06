#include <eve/movie_demo/rainy_movie_night_demo.hpp>
#include <eve/core/logging/logger.hpp>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    const std::string data_root = argc > 1 ? argv[1] : "data";

    eve::movie_demo::RainyMovieNightDemo demo;
    if (!demo.initialize(data_root)) {
        std::cerr << "Failed to initialize Rainy Movie Night demo\n";
        return 1;
    }

    EVE_LOG(Info, "MovieDemo", "Rainy Movie Night — cinematic vertical slice");

    auto begin = demo.begin_scene();
    std::cout << "[Begin] scene=" << begin.state << " shot=" << begin.camera_shot << "\n";

    for (int i = 0; i < 12 && begin.success; ++i) {
        auto beat = demo.advance_scene();
        if (!beat.dialogue.empty()) {
            std::cout << "[Dialogue] " << beat.dialogue << "\n";
        }
        if (!beat.camera_shot.empty()) {
            std::cout << "[Camera] " << beat.camera_shot << "\n";
        }
        if (beat.state == "completed") {
            std::cout << "[Scene] completed, memory=" << beat.memory_id << "\n";
            break;
        }
    }

    const std::string save_path = "/tmp/eve_movie_night_save.json";
    demo.save_state(save_path);
    demo.load_state(save_path);

    auto recall = demo.recall_movie_memory();
    std::cout << "[Memory] " << recall.dialogue << "\n";
    std::cout << demo.summarize() << "\n";

    demo.shutdown();
    return 0;
}
