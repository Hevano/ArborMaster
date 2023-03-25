#include "Adapter.h"

namespace ArborMasterAdapter {
  std::shared_ptr<TreeDesignNode> Adapter::getSubTree(json& data)
  {
    int id = data["id"].get<int>();
    auto name = data["name"].get<std::string>();
    std::shared_ptr<TreeDesignNode> root = std::make_shared<TreeDesignNode>(name, id);


    auto dump = data.dump();

    for (auto childJson : data["children"]) {
      root->children.push_back(getSubTree(childJson));
    }

    return root;
  }

  std::shared_ptr<TreeDesignNode> Adapter::loadTree(const std::string& path, std::string& debugPath)
  {
    std::ifstream stream(path);
    json data;
    stream >> data;

    debugPath = data["debugPath"].get<std::string>();

    return getSubTree(data["root"]);
  }
};
