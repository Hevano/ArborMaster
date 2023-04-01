#pragma once
#include "TreeNode.h"
#include "EditorNode.h"
#include "Blackboard.h"
#include "NodeFactory.h"

#include "imnodes.h"
#include "imnodes_internal.h"

#include <unordered_map>
#include <unordered_set>

namespace ArborMaster {
  class EditorTree
  {
    friend class TreeExporter;
    friend class EditorTreeTest;

  private:
    int m_editorId = 2; //Node Id 1 is reserved by the root
    Blackboard m_blackboard;
    std::unordered_map<int, EditorNode> m_editorNodes;
    std::unordered_set<int> m_freeNodes;
    std::unordered_map<int, EditorLink> m_editorLinks;
    std::unordered_map<int, std::vector<int>> m_adjList;

    std::string m_designPath;

  private:
    //Traverses entire tree to determine blackboard
    void updateBlackboard();

    //Updates blackboard with keys in single node
    void updateBlackboard(const TreeNode& node, bool added = true);

    //Adds newly created links and removes deleted nodes and links
    void updateTree();
    
    void deleteLink(EditorLink link);
    void deleteEditorNode(EditorNode node);

    void setNodeColor(const EditorNode& node);
    

  public:
    EditorTree() = default;
    EditorTree(
      const std::unordered_map<int, EditorNode>& editorNodes, 
      const std::unordered_set<int>& freeNodes, 
      const std::unordered_map<int, EditorLink>& editorLinks, 
      const std::unordered_map<int, std::vector<int>>& adjList
    );

    const Blackboard& getBlackboard() const;
    void drawDropZone(const NodeFactory& nodeCache);
    void draw(const NodeFactory& nodeCache);

    //Updates the running status of a node
    void updateNodeStatus(unsigned int nodeId, unsigned int status);

    const inline std::string& getPath() const
    {
      return m_designPath;
    }

    inline std::string& getPath()
    {
      return const_cast<std::string&>(const_cast<const EditorTree*>(this)->getPath());
    }
    void inline setPath(const std::string& path)
    {
      m_designPath = path;
    }

    int inline getNewId()
    {
      return ++m_editorId;
    }

    //Id Conversion Functions

    int inline getLinkParent(const EditorLink& link) const
    {
      return link.startId >> 16;
    }

    int inline getLinkChild(const EditorLink& link) const
    {
      return link.endId >> 8;
    }

    int inline getNodeInputId(const EditorNode& node) const
    {
      return node.id << 8;
    }

    int inline getNodeOutputId(const EditorNode& node)
    {
      return node.id << 16;
    }

  };
}
