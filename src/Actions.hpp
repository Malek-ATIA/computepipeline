#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <regex>
#include <any>

// Result object containing output and metadata
class Result {
public:
    std::string type;
    std::any data;
    int lastError; // Default is 0

    explicit Result(std::string t, std::any d, int e = 0) 
        : type(std::move(t)), data(std::move(d)), lastError(e) {}
};


// Abstract Base Class for Actions
class Action {
public:
    virtual Result execute(const Result& input) = 0;
    virtual std::string getType() const = 0;
    virtual ~Action() = default;
};

// Factory for creating actions dynamically
class ActionFactory {
public:
    using CreatorFunc = std::function<std::unique_ptr<Action>()>;
    static ActionFactory& getInstance() {
        static ActionFactory instance;
        return instance;
    }

    void registerAction(const std::string& actionType, CreatorFunc creator) {
        registry_[actionType] = std::move(creator);
    }

    std::unique_ptr<Action> createAction(const std::string& actionType) {
        auto it = registry_.find(actionType);
        if (it != registry_.end()) {
            return it->second();
        }
        throw std::runtime_error("Unknown Action Type: " + actionType);
    }

private:
    std::unordered_map<std::string, CreatorFunc> registry_;
    ActionFactory() = default;
};

// Load Action - Handles different URI types
class LoadAction : public Action {
    std::string uri_;
public:
    explicit LoadAction(std::string uri) : uri_(std::move(uri)) {}
    Result execute(const Result& input) override {
        if (uri_.find("file://") == 0) {
            std::cout << "Loading from local file: " << uri_ << std::endl;
        } else if (uri_.find("http://") == 0 || uri_.find("https://") == 0) {
            std::cout << "Downloading from URL: " << uri_ << std::endl;
        } else if (uri_.find("bundle://") == 0) {
            std::cout << "Loading from application bundle: " << uri_ << std::endl;
        } else {
            std::cout << "Unsupported loading method for uri: " << uri_ << std::endl;
            return Result(nullptr, uri_, -1);
        }
        return Result("raw_data", "raw_data_buffer");
    }
    std::string getType() const override { return "LoadAction"; }
};


// Default Actions (with factory registration)
class DecodeImage : public Action {
public:
    Result execute(const Result& input) override {
        std::cout << "Decoding Image..." << std::endl;
        return Result("decoded_image", "decoded_image_buffer");
    }
    std::string getType() const override { return "DecodeImage"; }
};



class DecompressData : public Action {
public:
    Result execute(const Result& input) override {
        std::cout << "Decompressing Data..." << std::endl;
        return Result("decompressed_data", "decompressed_data_buffer");
    }
    std::string getType() const override { return "DecompressData"; }
};


class ParseJson : public Action {
public:
    Result execute(const Result& input) override {
        std::cout << "Parsing JSON into C++ Object..." << std::endl;
        return Result("json_object", "json_object_buffer");
    }
    std::string getType() const override { return "ParseJson"; }
};


#endif // ACTIONS_HPP