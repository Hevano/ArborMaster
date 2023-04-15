#include "NodeImporter.h"
#include "TreeNode.h"

namespace ArborMaster
{
void NodeImporter::importAll(std::unordered_map<std::string, TreeNode>& importedNodes)
{
  std::ifstream sourceFile(m_path);
  std::string sourceLine;
  while (std::getline(sourceFile, sourceLine)) {
    try {
      TreeNode tn = importNode(sourceLine);
      importedNodes[tn.name] = tn;
    } catch (std::exception e) {
      std::cout << e.what() << std::endl;
    }
  }
}

TreeNode NodeImporter::importNode(const std::string& source)
{
  if (source.substr(0, NODE_FLAG_STRING.size()) == NODE_FLAG_STRING) {
    std::vector<std::string> splitString;
    int start = NODE_FLAG_STRING.size();
    int end = source.find(NODE_DELIMITER_STRING);
    while (end != -1) {
      splitString.push_back(source.substr(start, end - start));
      start = end + 1;
      end = source.find(NODE_DELIMITER_STRING, start);
    }
    if (splitString.size() < 2)
      throw std::exception("Not enough fields specified");
    TreeNode tn;
    tn.name = splitString[0];
    tn.childCap = std::stoi(splitString[1]);
    for (int i = 2; i < splitString.size(); i++) {
      tn.blackboardKeys.emplace(splitString[i]);
    }
    return tn;
  } else {
    throw std::exception("Source string not flagged");
  }
}
}