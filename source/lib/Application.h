#pragma once
#include "BehaviourTree.h"
#include "AIActor.h"
#include "NodeFactory.h"
#include "UIHelper.h"


#include <unordered_map>
#include <string>
#include "NodeImporter.h"
#include "TreeExporter.h"

namespace ArborMaster
{
  class Application
  {
    friend class UIHelper;
  private:
    BehaviourTree m_editorTree;
    std::unordered_map<std::string, AIActor> m_actors;
    NodeFactory m_nf;
    NodeImporter m_importer;
    TreeExporter m_exporter;
    UIHelper m_ui;

  public:
    void run();
    void setSourcePath(const std::string& path);
    void importNodes();
    void exportTree();
    void saveTree();
    void newTree();
  };
}
