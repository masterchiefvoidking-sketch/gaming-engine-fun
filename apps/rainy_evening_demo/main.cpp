#include <eve/rainy_demo/rainy_evening_demo.hpp>
#include <eve/core/logging/logger.hpp>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    const std::string data_root = argc > 1 ? argv[1] : "data";

    eve::rainy_demo::RainyEveningDemo demo;
    if (!demo.initialize(data_root)) {
        std::cerr << "Failed to initialize Rainy Evening demo\n";
        return 1;
    }

    EVE_LOG(Info, "RainyDemo", "Rainy Evening Apartment — vertical slice");

    auto talk = demo.talk_in_living_room();
    std::cout << "[Talk] " << talk.dialogue << "\n";

    auto compliment = demo.offer_compliment();
    std::cout << "[Compliment] " << compliment.dialogue << " (stage: " << compliment.stage
              << ")\n";

    auto dinner = demo.invite_to_dinner();
    std::cout << "[Dinner] " << dinner.dialogue << "\n";

    auto outfit = demo.trigger_outfit_dialogue();
    std::cout << "[Outfit] " << outfit.dialogue << "\n";

    auto blocked = demo.try_blocked_action("hug");
    std::cout << "[Boundary] hug allowed=" << blocked.success << " — " << blocked.rejection
              << "\n";

    const std::string save_path = "/tmp/eve_rainy_evening_save.json";
    demo.save_state(save_path);
    demo.load_state(save_path);

    auto recall = demo.recall_memory("compliment_received");
    std::cout << "[Memory] " << recall.dialogue << "\n";

    std::cout << demo.summarize() << "\n";
    demo.shutdown();
    return 0;
}
