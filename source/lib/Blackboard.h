#pragma once
#include <unordered_set>
#include <string>
namespace ArborMaster
{

//Represents the design-time keys in the blackboard
class Blackboard
{
public:
  std::unordered_set<std::string> data;

public:
  bool hasKey(const std::string & keyName) const;
};
}
	

