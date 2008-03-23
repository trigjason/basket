/***************************************************************************
 *   Copyright (C) 2005 by S�astien Laot                                 *
 *   slaout@linux62.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef TAGEDIT_H
#define TAGEDIT_H

#include <kdialogbase.h>
#include <kcombobox.h>
#include <q3listview.h>
#include <q3valuelist.h>
//Added by qt3to4:
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>

class Q3GroupBox;
class QLineEdit;
class QCheckBox;
class KPushButton;
class KKeyButton;
class KIconButton;
class KFontCombo;
class QLabel;
class KShortcut;

class KColorCombo2;

class FontSizeCombo;

class Tag;
class State;

class StateCopy
{
  public:
	typedef Q3ValueList<StateCopy*> List;
	StateCopy(State *old = 0);
	~StateCopy();
	State *oldState;
	State *newState;
	void copyBack();
};

class TagCopy
{
  public:
	typedef Q3ValueList<TagCopy*> List;
	TagCopy(Tag *old = 0);
	~TagCopy();
	Tag *oldTag;
	Tag *newTag;
	StateCopy::List stateCopies;
	void copyBack();
	bool isMultiState();
};

class TagListViewItem : public Q3ListViewItem
{
  public:
	TagListViewItem(Q3ListView     *parent, TagCopy *tagCopy);
	TagListViewItem(Q3ListViewItem *parent, TagCopy *tagCopy);
	TagListViewItem(Q3ListView     *parent, Q3ListViewItem *after, TagCopy *tagCopy);
	TagListViewItem(Q3ListViewItem *parent, Q3ListViewItem *after, TagCopy *tagCopy);
	TagListViewItem(Q3ListView     *parent, StateCopy *stateCopy);
	TagListViewItem(Q3ListViewItem *parent, StateCopy *stateCopy);
	TagListViewItem(Q3ListView     *parent, Q3ListViewItem *after, StateCopy *stateCopy);
	TagListViewItem(Q3ListViewItem *parent, Q3ListViewItem *after, StateCopy *stateCopy);
	~TagListViewItem();
	TagCopy*   tagCopy()   { return m_tagCopy;   }
	StateCopy* stateCopy() { return m_stateCopy; }
	bool isEmblemObligatory();
	TagListViewItem* lastChild();
	TagListViewItem* prevSibling();
	TagListViewItem* parent() const; // Reimplemented to cast the return value
	int width(const QFontMetrics &fontMetrics, const Q3ListView *listView, int column) const;
	void setup();
	void paintCell(QPainter *painter, const QColorGroup &colorGroup, int column, int width, int align);

  private:
	TagCopy   *m_tagCopy;
	StateCopy *m_stateCopy;
};

class TagListView : public Q3ListView
{
  Q_OBJECT
  public:
	TagListView(QWidget *parent = 0, const char *name = 0, Qt::WFlags flags = 0);
	~TagListView();
	void keyPressEvent(QKeyEvent *event);
	void contentsMouseDoubleClickEvent(QMouseEvent *event);
	void contentsMousePressEvent(QMouseEvent *event);
	void contentsMouseReleaseEvent(QMouseEvent *event);
	TagListViewItem* currentItem() const; // Reimplemented to cast the return value
	TagListViewItem* firstChild() const; // Reimplemented to cast the return value
	TagListViewItem* lastItem() const; // Reimplemented to cast the return value
  signals:
	void deletePressed();
	void doubleClickedItem();
};

/**
  * @author S�astien Laot
  */
class TagsEditDialog : public KDialogBase
{
  Q_OBJECT
  public:
	TagsEditDialog(QWidget *parent = 0, State *stateToEdit = 0, bool addNewTag = false);
	~TagsEditDialog();
	State::List deletedStates() { return m_deletedStates; }
	State::List addedStates()   { return m_addedStates;   }
	TagListViewItem* itemForState(State *state);
  private slots:
	void newTag();
	void newState();
	void moveUp();
	void moveDown();
	void deleteTag();
	void renameIt();
	void capturedShortcut(const KShortcut &shortcut);
	void removeShortcut();
	void removeEmblem();
	void modified();
	void currentItemChanged(Q3ListViewItem *item);
	void slotCancel();
	void slotOk();
	void selectUp();
	void selectDown();
	void selectLeft();
	void selectRight();
	void resetTreeSizeHint();
  private:
	void loadBlankState();
	void loadStateFrom(State *state);
	void loadTagFrom(Tag *tag);
	void saveStateTo(State *state);
	void saveTagTo(Tag *tag);
	void ensureCurrentItemVisible();
	TagListView   *m_tags;
	KPushButton   *m_moveUp;
	KPushButton   *m_moveDown;
	KPushButton   *m_deleteTag;
	QLineEdit     *m_tagName;
	KKeyButton    *m_shortcut;
	QPushButton   *m_removeShortcut;
	QCheckBox     *m_inherit;
	Q3GroupBox     *m_tagBox;
	Q3GroupBox     *m_stateBox;
	QLabel        *m_stateNameLabel;
	QLineEdit     *m_stateName;
	KIconButton   *m_emblem;
	QPushButton   *m_removeEmblem;
	QPushButton   *m_bold;
	QPushButton   *m_underline;
	QPushButton   *m_italic;
	QPushButton   *m_strike;
	KColorCombo2  *m_textColor;
	KFontCombo    *m_font;
	FontSizeCombo *m_fontSize;
	KColorCombo2  *m_backgroundColor;
	QLineEdit     *m_textEquivalent;
	QCheckBox     *m_onEveryLines;

	TagCopy::List m_tagCopies;
	State::List   m_deletedStates;
	State::List   m_addedStates;

	bool m_loading;
};

#endif // TAGEDIT_H
