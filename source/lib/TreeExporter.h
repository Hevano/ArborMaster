#pragma once
#include "BehaviourTree.h"

#include <string>

namespace ArborMaster {
class TreeExporter
{
private:
  std::string m_path;

public:
  const std::string& getPath() const;
  void setPath(const std::string& path);
  //Exports the tree to m_path for use in client code
  void exportTree(const BehaviourTree& bt) const;

  //saves the design of the tree to path in serialized json format
  void saveDesign(const BehaviourTree& bt, const std::string& path) const;

  //Loads the design of the tree from path
  BehaviourTree loadDesign(const std::string& path);

};
}

