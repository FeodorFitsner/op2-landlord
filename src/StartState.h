#pragma once

#include "NAS2D/NAS2D.h"

#include "EditorState.h"

// UI
#include "Button.h"
#include "ListBox.h"
#include "TextField.h"


using namespace std;
using namespace NAS2D;


/**
 * \class StartState
 * \brief Implements a startup state for the CoM Map Editor.
 */
class StartState: public State
{
public:

	StartState();
	~StartState();

protected:

	void initialize();

	State* update();

	void onKeyDown(NAS2D::EventHandler::KeyCode key, NAS2D::EventHandler::KeyModifier mod, bool repeat);
	void onMouseMove(int x, int y, int relX, int relY);
	void onDoubleClick(EventHandler::MouseButton button, int x, int y);

	void onQuit();

private:
	StringList getFileList(const std::string& directory);

	void fillMapMenu();

	void button_CreateNew_click();
	void button_LoadExisting_click();
	void button_RefreshLists_click();

private:
	Timer			mTimer;					/**< used to flash messages. */

	Font			mFont;					/**< Internal Font to use. */

	Point_2d		mMouseCoords;			/**< Mouse pointer coordinates. */

	Rectangle_2d	mLayoutRect;			/**< Rectangle used to position UI elements on screen. */

	Button			mBtnCreateNew;			/**< Create New button. */
	Button			mBtnLoadExisting;		/**< Load Existing button. */
	Button			mBtnRefreshLists;		/**< Refresh File Lists button. */

	TextField		mTxtWidth;				/**< Map Width text field. */
	TextField		mTxtHeight;				/**< Map Height text field. */
	TextField		txtMapPath;				/**< Filename for the map. */

	ListBox			mMapFilesMenu;			/**< Map File List menu. */

	bool			mScanningMaps = true;	/**<  */

	State*			mReturnState = this;	/**< State to return during updates. */
};
