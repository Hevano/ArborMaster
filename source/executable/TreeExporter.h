#pragma once
#include "BehaviourTree.h"

#include <string>

namespace ArborMaster {
class TreeExporter
{
public:
  std::string sourcePath;

private:
  void exportTree(const BehaviourTree& bt) const;
  void saveDesign(const BehaviourTree& bt, const std::string& path) const;
  BehaviourTree loadDesign(const std::string& path);
};
}

