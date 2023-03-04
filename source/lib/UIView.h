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

	public:
		std::function<void(void)> exportCallback;
		std::function<void(void)> importCallback;
		std::function<void(void)> saveCallback;
		std::function<void(void)> loadCallback;
		std::function<void(void)> newTreeCallback;

	public:
		void drawExportPopup(std::string& path);
		void drawImportPopup(std::string& path);
		void drawNewPopup(std::string& path);
		void drawOpenPopup(std::string& path);
		void drawSaveAsPopup(std::string& path);

		//Returns true if popup was opened
		bool drawToolbar(std::string& exportPath, std::string& importPath, std::string& savePath);
		void drawTabs();
		void drawNodeList(const NodeFactory& nodeCache);
		void drawNode(const TreeNode& n, bool draggable = false);
		void drawBlackboard(const EditorTree& tree);

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

