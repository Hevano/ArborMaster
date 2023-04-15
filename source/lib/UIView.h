#pragma once

#include <functional>
#include <string>

namespace ArborMaster {
	class NodeFactory;
	class TreeNode;
	class EditorTree;
	class UIView {
	private:
		enum class ToolBarActions
		{
		None,
		New,
		Save,
		SaveAs,
		Open,
		Export,
		Import
		};

		inline static const std::unordered_map<ToolBarActions, std::string> POPUP_NAMES {
			{ ToolBarActions::New, "New Tree Design" },
			{ ToolBarActions::Open, "Open Tree Design" },
			{ ToolBarActions::SaveAs, "Save As" },
			{ ToolBarActions::Export, "Tree Export" },
			{ ToolBarActions::Import, "Tree Import" },
		};

		bool m_alertTriggered = false;
		std::string m_alertMsg = "";

	public:
		std::function<void(void)> exportCallback;
		std::function<void(void)> importCallback;
		std::function<void(void)> saveCallback;
		std::function<void(void)> loadCallback;
		std::function<void(void)> newTreeCallback;
		std::function<void(void)> debuggerCallback;
		std::function<bool(unsigned int)> actorClickCallback;

	public:
		void drawExportPopup(std::string& path);
		void drawImportPopup(std::string& path);
		void drawNewPopup(std::string& path);
		void drawOpenPopup(std::string& path);
		void drawSaveAsPopup(std::string& path);
		void drawAlertPopup();

		//Returns true if popup was opened
		bool drawToolbar(std::string& exportPath, std::string& importPath, std::string& savePath);
		void drawActorList(const std::unordered_map<unsigned int, std::string>& actorMap);
		void drawTabs(const std::string& tabName);
		void drawNodeList(const NodeFactory& nodeCache);
		void drawNode(const TreeNode& n, bool draggable = false);
		void drawBlackboard(const EditorTree& tree);
		void drawBlackboard(const std::unordered_map<std::string, std::string>& debugBlackboard);
		void pollHotKeys();
		void triggerAlert(const std::string msg);

	private:
		void drawPopup(
			const std::string& name,
			const std::string& msg,
			std::string& path,
			std::function<void(void)> callback,
			std::string fieldText = ""
		);

	};
}

