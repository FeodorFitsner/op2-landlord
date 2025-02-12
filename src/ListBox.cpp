#include "ListBox.h"

#include "Common.h"


/**
 * C'tor
 */
ListBox::ListBox():	mText(COLOR_WHITE),
					mHighlightBg(COLOR_GREEN),
					mHighlightText(COLOR_WHITE)
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &ListBox::onMouseDown);
	Utility<EventHandler>::get().mouseMotion().connect(this, &ListBox::onMouseMove);
	Utility<EventHandler>::get().mouseWheel().connect(this, &ListBox::onMouseWheel);
	
	init();
}


/**
 * D'tor
 */
ListBox::~ListBox()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &ListBox::onMouseDown);
	Utility<EventHandler>::get().mouseMotion().disconnect(this, &ListBox::onMouseMove);
	Utility<EventHandler>::get().mouseWheel().disconnect(this, &ListBox::onMouseWheel);

	mSlider.change().disconnect(this, &ListBox::slideChanged);
}

/**
*
*/
void ListBox::init()
{
	mSlider.length(0);
	mSlider.thumbPosition(0);
	mSlider.change().connect(this, &ListBox::slideChanged);
}


void ListBox::size(float w, float h)
{
	Control::size(w, h);
	deleteControl("mSlider");
	addControl("mSlider", &mSlider, rect().width() - 14, 0);
	mSlider.font(font());
	mSlider.displayPosition(false);
	mSlider.size(14, rect().height());
	mDisplayLines = static_cast<int>(rect().height() / mLineHeight);
	_checkSlider();
}


void ListBox::onFontChanged()
{
	mLineHeight = font().height() + 2;
	mDisplayLines = static_cast<int>(rect().height() / mLineHeight);
}


/**
 * Gets whether the menu is empty or not.
 */
bool ListBox::empty() const
{
	return mItems.empty();
}


/**
 * Adds an item to the Menu.
 *
 * \param	item	Item to add.
 *
 * \warning	Menu::font(Font& font) must have been called with a valid Font
 *			before this function can be safely called.
 *
 * \todo	Make this function safe to call regardless of whether a font
 *			has been defined or not.
 */
void ListBox::addItem(const std::string& item)
{
	mItems.push_back(item);
	sort();
	_checkSlider();
}


/**
 * Removes a named item from the Menu.
 *
 * \todo	Complete this function.
 *
 * \note	Safe to call if Menu is empty.
 */
void ListBox::removeItem(const std::string& item)
{
	// Ignore if menu is empty
	if(empty())
		return;

	StringList::iterator it = mItems.begin();

	while(it != mItems.end())
	{
		if(toLowercase((*it)) == toLowercase(item))
		{
			mItems.erase(it);
			mCurrentSelection = NO_SELECTION;
			return;
		}

		++it;
	}

	sort();
	_checkSlider();
}


bool ListBox::itemExists(const std::string& item)
{
	// Ignore if menu is empty
	if(empty())
		return false;

	for(size_t i = 0; i < mItems.size(); i++)
	{
		if(toLowercase(mItems[i]) == toLowercase(item))
			return true;
	}

	return false;
}


/**
 * Drops all items from the list.
 */
void ListBox::dropAllItems()
{
	mItems.clear();
	mCurrentSelection = 0;
	_checkSlider();
}


void ListBox::onMouseDown(EventHandler::MouseButton button, int x, int y)
{
	// Ignore if menu is empty or invisible
	if (empty() || !visible()) { return; }

	if (!isPointInRect(Point_2d(x, y), rect())) { return; }

	if (mSlider.visible() && isPointInRect(Point_2d(x, y), mSlider.rect()))
		return;		// if the mouse is on the slider then the slider should handle that

	int idx = ((y - (int)rect().y()) / (font().height() + 2)) % ((int)rect().height() / (font().height() + 2)) + mCurrentOffset;
	currentSelection(idx);
}


void ListBox::onMouseMove(int x, int y, int relX, int relY)
{
	// Ignore if menu is empty or invisible
	if (empty() || !visible()) { return; }

	mMouseCoords(x, y);
}


void ListBox::_checkSlider()
{
	mItemMin = 0;
	mItemMax = mItems.size();

	if ((mLineHeight * mItems.size()) > rect().height())
	{
		if (mDisplayLines < static_cast<int>(mItems.size()))
		{
			mSlider.length(mItems.size() - mDisplayLines);
			mSlider.visible(true);
			mCurrentOffset = mSlider.thumbPosition();
			mItemMin = mCurrentOffset;
			mItemMax = mCurrentOffset + mDisplayLines;
			mItemWidth = rect().width() - mSlider.rect().width();
		}
	}
	else
	{
		mSlider.visible(false);
	}
}


void ListBox::update()
{
	// Ignore if menu is empty or invisible
	if (empty() || !visible()) { return; }

	Renderer& r = Utility<Renderer>::get();

	r.drawBoxFilled(rect(), 0, 0, 0);

	int itemY;

	// draw boundaries of the widget
	r.drawBox(rect().x(), rect().y(), mItemWidth, rect().height(), 0, 0, 0, 100);
	r.drawBoxFilled(rect().x(), rect().y(), mItemWidth, rect().height(), 225, 225, 0, 85);

	// Highlight currently selected file
	if (mItemMin <= mCurrentSelection && mCurrentSelection < mItemMax)
	{
		itemY = rect().y() + ((mCurrentSelection - mCurrentOffset)  * mLineHeight);
		r.drawBoxFilled(rect().x(), itemY, mItemWidth, mLineHeight, mHighlightBg.red(), mHighlightBg.green(), mHighlightBg.blue(), 80);
	}

	// display actuals values that are ment to be
	for (int i = mItemMin; i < mItemMax; i++)
	{
		itemY = rect().y() + ((i - mItemMin) * mLineHeight);
		r.drawTextShadow(font(), mItems[i], rect().x(), itemY, 1, mText.red(), mText.green(), mText.blue(), 0, 0, 0);
	}

	r.drawBox(rect(), 0, 0, 0);

	// draw the slider if needed
	mSlider.update();
}


void ListBox::onMouseWheel(int x, int y)
{
	if (!isPointInRect(mMouseCoords, rect())) { return; }

	if (y < 0)
	{
		mSlider.changeThumbPosition(1.0);
	}
	else if (y > 0)
	{
		mSlider.changeThumbPosition(-1.0);
	}
}


void ListBox::slideChanged(double _position)
{
	_checkSlider();

	int pos = static_cast<int>(_position);
	if (static_cast<float>(pos) != _position)
		mSlider.thumbPosition(static_cast<double>(pos));
}