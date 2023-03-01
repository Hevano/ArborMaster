#pragma once

#include <functional>

namespace ArborMaster {
	class NodeCache;
	class TreeNode;
	class EditorTree;
	class UIView {
	private:
		inline static const std::string EXPORT_POPUP_NAME = "Tree Export";
		inline static const std::string IMPORT_POPUP_NAME = "Node Import";
		inline static const std::string OPEN_POPUP_NAME = "Open Tree Design";
		inline static const std::string NEW_POPUP_NAME = "New Tree Design";
		inline static const std::string SAVE_AS_POPUP_NAME = "Save As";

	public:
		std::function<void(void)> exportCallback;
		std::function<void(void)> importCallback;
		std::function<void(void)> saveCallback;
		std::function<void(void)> loadCallback;

	public:
		void drawExportPopup(std::string& path);
		void drawImportPopup(std::string& path);
		void drawNewPopup(std::string& path);
		void drawLoadPopup(std::string& path);
		void drawSaveAsPopup(std::string& path);

		void drawToolbar();
		void drawTabs();
		void drawNodeList(NodeCache nodes);
		void drawNode(const TreeNode& n, bool draggable = false);
		void drawBlackboard(const EditorTree& tree);
	};
}

