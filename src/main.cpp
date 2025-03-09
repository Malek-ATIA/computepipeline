#include "ComputePipeline.hpp"

int main() {
    try {
        std::string uri = "file://path/to/data.png";  // Example URI

        // Get the appropriate pipeline instance based on the file type
        ComputePipelineBase& pipeline = ComputePipelineManager::getInstance().getPipeline(uri);

        // Execute the pipeline
        pipeline.execute();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
