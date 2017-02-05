#pragma once

#include "NAS2D/NAS2D.h"

#include "Tileset.h"

#include "Button.h"

#include "Pattern.h"

using namespace NAS2D;

/**
 * \class TilePalette
 * \brief Implements a basic tile palette window.
 * 
 * TilePalette assumes that the tileset pointer points to a valid location in
 * memory. The pointer may be NULL in which case the TilePalette will assume
 * a non-functional state (won't come out of hiding.
 * 
 * TilePalette aligns itself to the bottom right corner of the screen regarless
 * of resolution. It can't be moved around -- makes things a lot easier.
 */
class TilePalette
{
public:
	TilePalette();

	// Sets the internal tileset pointer.
	void tileset(Tileset* tset);

	void update();

	void hidden(bool hidden);
	bool hidden() const { return mHidden; }

	void font(Font& font);

	bool patternFill() const { return mPatternFill; }

	const Pattern& pattern() const { return mBrushPattern; }

	const Rectangle_2d& rect() const { return mRect; }
	const Rectangle_2d& closeRect() const { return mCloseRect; }

	int index() const { return mTsetIndex; }

	void onMouseDown(MouseButton button, int x, int y);
	void onMouseUp(MouseButton button, int x, int y);

	void onMouseMove(int x, int y, int relX, int relY);

private:

	typedef std::vector<std::vector<Rectangle_2d> > RectList;


	void button_Prev_click();
	void button_Next_click();
	void button_FloodFill_click();

	void determinePattern();

	Rectangle_2d getRectFromPoints(const Point_2d& pt1, const Point_2d& pt2);


	Font*			mFont;				/**< Font to use to draw basic information. */
	Tileset*		mTset;				/**< Pointer to a Tileset. TilePalette does not own this Tileset object. */

	Rectangle_2d	mRect;				/**< Area and position of the palette. */
	Rectangle_2d	mCloseRect;			/**< Area of the 'close button' */
	Rectangle_2d	mTileGridRect;		/**< Area that the tiles are over. */

	RectList		mSlots;				/**< List of rectangles for each slot. */

	Button			mBtnPrev;			/**<  */
	Button			mBtnNext;			/**<  */
	Button			mBtnFloodFill;		/**< Pattern fill button. */

	Image			mShow;				/**< Displayed in the title area when the palette is hidden. */
	Image			mHide;				/**< Displayed in the title area when the palette is not hidden. */

	Pattern			mBrushPattern;		/**< Brush pattern for stamp and pattern floods. */

	int				mNumPages;			/**< Number of pages the palette needs to show. */
	int				mCurrentPage;		/**< Current tile page. */
	int				mTsetIndex;			/**< Currently selected tileset index. */

	Point_2d		mStartIndex;		/**< Index of the selected tile(s). */
	Point_2d		mEndIndex;			/**< Index of the selected tile(s). */

	Point_2d		mMouseCoords;		/**< Point where a drag started. */
	Point_2d		mDragOrigin;		/**< Point where a drag started. */

	bool			mHidden;			/**< Flag indicating that the palette shouldn't be drawn. */
	bool			mLeftButtonDown;	/**< Flag indicating that the left mouse button is depressed. */
	bool			mPatternFill;		/**< Flag indicating that we're in pattern fill mode. */
	bool			mMouseOverTiles;	/**< Flag indicating that the mouse is actually over the tiles. */

};
