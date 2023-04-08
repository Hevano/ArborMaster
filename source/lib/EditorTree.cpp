#include "EditorTree.h"
#include "NodeFactory.h"

#include <algorithm>
#include <format>

void ArborMaster::EditorTree::updateBlackboard()
{
  m_blackboard.data.clear();
  for (auto& [id, node] : m_editorNodes) {
    updateBlackboard(*node.treeNode);
  }
}

void ArborMaster::EditorTree::updateBlackboard(const TreeNode& node, bool added)
{
  if (added) {
    for (const auto& key : node.blackboardKeys) {
      m_blackboard.data.emplace(key);
    }
  }
  else {
    //Remove must be called after node is removed from editorNodes dict
    std::unordered_set<std::string> toDelete(node.blackboardKeys);
    for (const auto& [id, n] : m_editorNodes) {
      for (const auto& key : n.treeNode->blackboardKeys) {
        toDelete.erase(key);
      }
      if (toDelete.size() == 0) return; //If we run out of keys to delete, we are done
    }

    for (const auto& key : toDelete) {
      m_blackboard.data.erase(key);
    }
  }
}

void ArborMaster::EditorTree::updateTree()
{
  //Add new links
  EditorLink link;
  if (ImNodes::IsLinkCreated(&link.startId, &link.endId)) {
    int parentId = getLinkParent(link);
    int childId = getLinkChild(link);

    if (!m_freeNodes.contains(childId)) {
      auto oldLinkIt = m_editorLinks.end();
      for (auto existingLinkIt = m_editorLinks.begin(); existingLinkIt != m_editorLinks.end(); existingLinkIt++) {
        if (getLinkChild(existingLinkIt->second) == childId) {
          oldLinkIt = existingLinkIt;
        }
      }
      if (oldLinkIt != m_editorLinks.end()) {
        deleteLink(oldLinkIt->second);
      }
    }
    m_adjList[parentId].push_back(childId);
    link.id = getNewId();
    m_editorLinks[link.id] = link;
    m_freeNodes.erase(childId);
  }

  //Remove existing links
  int link_id;
  if (ImNodes::IsLinkDestroyed(&link_id)) {
    if (m_editorLinks.count(link_id) != 1) {
      throw std::exception("Tried to remove link not in map");
    }
    deleteLink(m_editorLinks[link_id]);
  }

  // Delete nodes
  int numSelected = ImNodes::NumSelectedNodes();
  if (numSelected != 0 && ImGui::IsKeyPressed(ImGuiKey_Delete, false)) {
    int* selectedIds = new int(numSelected);
    ImNodes::GetSelectedNodes(selectedIds);
    if (selectedIds != nullptr) {
      for (int i = 0; i < numSelected; i++) {
        deleteEditorNode(m_editorNodes[selectedIds[i]]);
      }
    }
  }
}

void ArborMaster::EditorTree::drawDropZone(const NodeFactory& nodeCache)
{
  if (ImGui::GetCurrentContext()->DragDropActive) {
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
    ImGui::InvisibleButton("##canvas", ImGui::GetWindowSize());
    if (ImGui::BeginDragDropTarget()) {
      if (const ImGuiPayload* payload =
        ImGui::AcceptDragDropPayload("NODE_DROP"))
      {
        IM_ASSERT(payload->DataSize == sizeof(std::string));
        std::string droppedTree = *(const std::string*)payload->Data;
        int id = getNewId();
        m_editorNodes.emplace(id, EditorNode(nodeCache.getNodes().at(droppedTree), ImGui::GetMousePos(), id));
        m_freeNodes.emplace(id);
        updateBlackboard(*(m_editorNodes.at(id).treeNode));
        ImNodes::SetNodeScreenSpacePos(id, ImGui::GetMousePos());
        ImNodes::SnapNodeToGrid(id);
      }
      ImGui::EndDragDropTarget();
    }
    ImGui::PopStyleColor();
  }
}

void ArborMaster::EditorTree::deleteLink(EditorLink link)
{
  if (!m_editorLinks.contains(link.id)) return;
  int parentId = getLinkParent(link);
  int childId = getLinkChild(link);
  if (m_adjList.contains(parentId)) {
    auto& childList = m_adjList.at(parentId);
    auto it = std::find(childList.begin(), childList.end(), childId);
    if (it != childList.end()) {
      m_freeNodes.emplace(childId);
      childList.erase(it);
    }
  }
  m_editorLinks.erase(link.id);
}

void ArborMaster::EditorTree::deleteEditorNode(EditorNode node)
{
  if (m_freeNodes.contains(node.id)) {
    m_freeNodes.erase(node.id);
  }
  else {
    int parentId = -1;
    int linkId = -1;

    std::vector<int> childLinks;
    //Find the link, if any, where this node is the child
    for (auto& [id, link] : m_editorLinks) {
      if (getLinkChild(link) == node.id) {
        linkId = link.id;
        parentId = getLinkParent(link);
      } else if(getLinkParent(link) == node.id) { //Also, save any child links of this node
        childLinks.push_back(link.id);
      }
    }
    //If such a link is found, delete this node from it's parent's adjacency list and remove the link
    if (parentId != -1 && linkId != -1) {
      auto it = std::find(m_adjList[parentId].begin(), m_adjList[parentId].end(), node.id);
      m_adjList[parentId].erase(it);
      m_editorLinks.erase(linkId);
    }
    //Remove any child links the node has and clear it from the adjacency list
    if (m_adjList.contains(node.id)) {
      for (auto childLinkId : childLinks) {
        deleteLink(m_editorLinks[childLinkId]);
      }
      m_adjList.erase(node.id);
    }
  }
  m_editorNodes.erase(node.id);
  updateBlackboard(*node.treeNode, false);
}

void ArborMaster::EditorTree::setNodeColor(const EditorNode& node)
{
  unsigned int color = 0;
  unsigned int colorSelected = 0;
  switch (node.staus) {
  case EditorNode::NodeStatus::FAILED:
    color = IM_COL32(255, 0, 0, 255);
    colorSelected = IM_COL32(255, 20, 20, 255);
    break;
  case EditorNode::NodeStatus::SUCCEEDED:
    color = IM_COL32(0, 200, 0, 255);
    colorSelected = IM_COL32(20, 200, 20, 255);
    break;
  case EditorNode::NodeStatus::RUNNING:
    color = IM_COL32(0, 0, 255, 255);
    colorSelected = IM_COL32(20, 20, 255, 255);
    break;
  }
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, color);
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, colorSelected);
}

ArborMaster::EditorTree::EditorTree(
  const std::unordered_map<int, EditorNode>& editorNodes, 
  const std::unordered_set<int>& freeNodes, 
  const std::unordered_map<int, EditorLink>& editorLinks, 
  const std::unordered_map<int, std::vector<int>>& adjList)
  : m_editorNodes(editorNodes)
  , m_freeNodes(freeNodes)
  , m_editorLinks(editorLinks)
  , m_adjList(adjList)
{
  updateBlackboard();
}

const ArborMaster::Blackboard& ArborMaster::EditorTree::getBlackboard() const
{
  return m_blackboard;
}

void ArborMaster::EditorTree::draw(const NodeFactory& nodeCache)
{
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration
    | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
    | ImGuiWindowFlags_NoBringToFrontOnFocus;
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
  ImGui::Begin("Node Editor", nullptr, windowFlags);
  ImNodes::BeginNodeEditor();

  drawDropZone(nodeCache);

  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(255, 0, 0, 255));
  ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(255, 20, 20, 255));

  //Draw Root Node
  ImNodes::BeginNode(1);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted("root");
  ImNodes::EndNodeTitleBar();

  ImNodes::BeginOutputAttribute(1 << 16);
  ImNodes::EndOutputAttribute();
  ImNodes::EndNode();

  ImNodes::PopColorStyle();
  ImNodes::PopColorStyle();

  // Draw nodes connected to the tree
  for (auto& [parentId, childList] : m_adjList) {
    int childCount = 1;

    std::sort(childList.begin(), childList.end(), [this](int i, int j)
    {
      return m_editorNodes.at(i).position.y < m_editorNodes.at(j).position.y;
    });

    for (auto& childId : childList) {
      setNodeColor(m_editorNodes.at(childId));
      ImNodes::BeginNode(childId);

      ImNodes::BeginNodeTitleBar();
      ImGui::TextUnformatted(std::format("({}) {}", childCount++, m_editorNodes.at(childId).treeNode->name).c_str());
      ImNodes::EndNodeTitleBar();

      ImNodes::BeginInputAttribute(getNodeInputId(m_editorNodes.at(childId)));
      ImNodes::EndInputAttribute();

      ImNodes::BeginOutputAttribute(getNodeOutputId(m_editorNodes.at(childId)));
      if (m_adjList[childId].size() > m_editorNodes.at(childId).treeNode->childCap)
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
      ImGui::Text(std::format("children {}/{}", m_adjList[childId].size(), m_editorNodes.at(childId).treeNode->childCap).c_str());
      if (m_adjList[childId].size() > m_editorNodes.at(childId).treeNode->childCap)
        ImGui::PopStyleColor();
      ImNodes::EndOutputAttribute();
      ImNodes::EndNode();

      ImNodes::PopColorStyle();
      ImNodes::PopColorStyle();

      m_editorNodes.at(childId).position = ImNodes::GetNodeGridSpacePos(childId);
    }
  }

  //Draw remaining nodes
  for (auto id : m_freeNodes) {
    auto editorNode = m_editorNodes[id];
    ImNodes::BeginNode(id);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(editorNode.treeNode->name.c_str());
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(getNodeInputId(editorNode));
    ImNodes::EndInputAttribute();

    ImNodes::BeginOutputAttribute(getNodeOutputId(editorNode));
    ImGui::Text("children");
    ImNodes::EndOutputAttribute();
    ImNodes::EndNode();
  }

  //Draw all links
  for (const auto& [id, editorLink] : m_editorLinks) {
    ImNodes::Link(id, editorLink.startId, editorLink.endId);
  }

  ImNodes::EndNodeEditor();
  ImGui::End();

  updateTree();
}

void ArborMaster::EditorTree::updateNodeStatus(unsigned int nodeId, unsigned int status)
{
  if (m_editorNodes.contains(nodeId) && status >= 0 && status <= 2) {
    m_editorNodes[nodeId].staus = static_cast<EditorNode::NodeStatus>(status);
  }
}
