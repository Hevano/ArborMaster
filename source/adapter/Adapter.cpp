#include "Adapter.h"

namespace ArborMasterAdapter {
  std::shared_ptr<TreeDesignNode> Adapter::getSubTree(json& data)
  {
    try {
      int id = data["id"].get<int>();
      auto name = data["name"].get<std::string>();
      std::shared_ptr<TreeDesignNode> root = std::make_shared<TreeDesignNode>(name, id);

      for (auto childJson : data["children"]) {
        auto child = getSubTree(childJson);
        if (child) {
          root->children.push_back(getSubTree(childJson));
        }
      }

      return root;
    }
    catch (std::exception e) {
      return nullptr;
    }
  }

  std::shared_ptr<TreeDesignNode> Adapter::loadTree(const std::string& path, std::string& debugPath)
  {
    if (!std::filesystem::exists(path)) return nullptr;
    try {
      std::ifstream stream(path);
      json data;
      stream >> data;
      debugPath = data["debugPath"].get<std::string>();

      return getSubTree(data["root"]);
    }
    catch (std::exception e) {
      return nullptr;
    }
  }
};
