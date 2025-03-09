/* ComputePipeline.hpp */
#ifndef COMPUTE_PIPELINE_HPP
#define COMPUTE_PIPELINE_HPP

#include "Actions.hpp"
#include <unordered_map>

// Base class for pipelines
class ComputePipelineBase {
public:
    virtual void execute(bool bStartFromScratch = false) = 0;
    virtual ~ComputePipelineBase() = default;
};

// Define specific pipeline items
class PipelineItem {
public:
    static void registerActions();
    static std::vector<std::string> queryNextActions(const std::string& currentAction);
};

class ImagePipelineItem : public PipelineItem {
public:
    static void registerActions() {
        ActionFactory::getInstance().registerAction("DecodeImage", []() { return std::make_unique<DecodeImage>(); });
    }

    static std::vector<std::string> queryNextActions(const std::string& currentAction) {
        if (currentAction == "LoadAction") {
            return {"DecodeImage"};
        }
        return {}; // No further actions
    }
};

class CompressedPipelineItem : public PipelineItem {
public:
    static void registerActions() {
        ActionFactory::getInstance().registerAction("DecompressData", []() { return std::make_unique<DecompressData>(); });
    }

    static std::vector<std::string> queryNextActions(const std::string& currentAction) {
        if (currentAction == "LoadAction") {
            return {"DecompressData"};
        }
        return {}; // No further actions
    }
};

class JsonPipelineItem : public PipelineItem {
public:
    static void registerActions() {
        ActionFactory::getInstance().registerAction("ParseJson", []() { return std::make_unique<ParseJson>(); });
    }

    static std::vector<std::string> queryNextActions(const std::string& currentAction) {
        if (currentAction == "LoadAction") {
            return {"ParseJson"};
        }
        return {}; // No further actions
    }
};

// Template for different pipeline types
template <typename PipelineItem>
class ComputePipeline : public ComputePipelineBase {
public:
    explicit ComputePipeline(const std::string& uri) : uri_(uri), lastResult_("initial", "") {
        std::cout << "ComputePipeline created for " << uri << std::endl;
        PipelineItem::registerActions();
        actions_.emplace_back(std::make_unique<LoadAction>(uri));
    }

    void execute(bool bStartFromScratch = false) {
        std::cout << "Executing pipeline actions...\n";
        if (bStartFromScratch) {
            lastResult_ = Result("initial", "");
        }

        int lastError = 0;
        while (!actions_.empty()) {
            auto action = std::move(actions_.front());
            actions_.erase(actions_.begin());
            Result result = action->execute(lastResult_);
            if (result.lastError == 0) {
                lastResult_ = result;
            } else {
                std::cout << "Error executing pipeline for uri " << uri_ << ". Quitting pipeline\n";
                lastError = result.lastError;
                break;
            }
            addNextAction(action->getType());
        }
        std::cout << "Executing pipeline actions completed: status code is " << lastError << ".\n";
    }

    void addNextAction(const std::string& currentAction) {
        std::vector<std::string> nextActions = PipelineItem::queryNextActions(currentAction);
        for (const auto& actionType : nextActions) {
            actions_.emplace_back(ActionFactory::getInstance().createAction(actionType));
        }
    }

private:
    std::string uri_;
    std::vector<std::unique_ptr<Action>> actions_;
    Result lastResult_;  // Store last successful action result
};

// Centralized Manager for ComputePipeline
class ComputePipelineManager {
public:
    static ComputePipelineManager& getInstance() {
        static ComputePipelineManager instance;
        return instance;
    }

    ComputePipelineBase& getPipeline(const std::string& uri) {
        if (pipelines_.find(uri) == pipelines_.end()) {
            pipelines_[uri] = createPipeline(uri);
        }
        return *pipelines_[uri];
    }

private:
    std::unordered_map<std::string, std::unique_ptr<ComputePipelineBase>> pipelines_;

    static std::unique_ptr<ComputePipelineBase> createPipeline(const std::string& uri) {
        if (std::regex_match(uri, std::regex(R"(.*\.(jpg|png|bmp)$)"))) {
            return std::make_unique<ComputePipeline<ImagePipelineItem>>(uri);
        } else if (std::regex_match(uri, std::regex(R"(.*\.(zip|gz|tar)$)"))) {
            return std::make_unique<ComputePipeline<CompressedPipelineItem>>(uri);
        } else if (std::regex_match(uri, std::regex(R"(.*\.(json)$)"))) {
            return std::make_unique<ComputePipeline<JsonPipelineItem>>(uri);
        }
        throw std::runtime_error("Unsupported file type for URI: " + uri);
    }

    ComputePipelineManager() = default;
};


#endif // COMPUTE_PIPELINE_HPP
