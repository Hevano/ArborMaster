#include "TreeNode.h"

namespace ArborMaster
{
void TreeNode::reorderChild(int oldIndex, int newIndex) {}
bool TreeNode::insertChild(const TreeNode& t, int index) {
    children.push_back(std::make_shared<TreeNode>(TreeNode(t)));//performs a copy
	return false;
}
void TreeNode::removeChild(const int index) {}
}