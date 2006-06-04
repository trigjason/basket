/***************************************************************************
 *   Copyright (C) 2003 by S�bastien Lao�t                                 *
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

#include <qlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qsizegrip.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qsizepolicy.h>
#include <kopenwith.h>
#include <klocale.h>
#include <qwhatsthis.h>
#include <kiconview.h>
#include <kiconloader.h>
#include <qdragobject.h>

#include "variouswidgets.h"

/** class RunCommandRequester: */

RunCommandRequester::RunCommandRequester(const QString &runCommand, const QString &message, QWidget *parent, const char *name)
 : QWidget(parent, name)
{
	m_message = message;

	QHBoxLayout *layout = new QHBoxLayout(this, /*margin=*/0, KDialogBase::spacingHint());
	m_runCommand        = new QLineEdit(runCommand, this);
	QPushButton *pb     = new QPushButton(/*"C&hoose..."*/i18n("..."), this);

	pb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	layout->addWidget(m_runCommand);
	layout->addWidget(pb);

	connect( pb, SIGNAL(clicked()), this, SLOT(slotSelCommand()) );
}

RunCommandRequester::~RunCommandRequester()
{
}

void RunCommandRequester::slotSelCommand()
{
	KOpenWithDlg *dlg =  new KOpenWithDlg(KURL::List(), m_message, m_runCommand->text(), this);
	dlg->exec();
	if ( ! dlg->text().isEmpty() )
		m_runCommand->setText(dlg->text());
}

QString RunCommandRequester::runCommand()
{
	return m_runCommand->text();
}

void RunCommandRequester::setRunCommand(const QString &runCommand)
{
	m_runCommand->setText(runCommand);
}

/** class IconSizeCombo: */

IconSizeCombo::IconSizeCombo(bool rw, QWidget *parent, const char *name)
 : QComboBox(rw, parent, name)
{
	insertItem(i18n("16 by 16 pixels"));
	insertItem(i18n("22 by 22 pixels"));
	insertItem(i18n("32 by 32 pixels"));
	insertItem(i18n("48 by 48 pixels"));
	insertItem(i18n("64 by 64 pixels"));
	insertItem(i18n("128 by 128 pixels"));
	setCurrentItem(2);
}

IconSizeCombo::~IconSizeCombo()
{
}

int IconSizeCombo::iconSize()
{
	switch (currentItem()) {
		default:
		case 0: return 16;
		case 1: return 22;
		case 2: return 32;
		case 3: return 48;
		case 4: return 64;
		case 5: return 128;
	}
}

void IconSizeCombo::setSize(int size)
{
	switch (size) {
		default:
		case 16:  setCurrentItem(0); break;
		case 22:  setCurrentItem(1); break;
		case 32:  setCurrentItem(2); break;
		case 48:  setCurrentItem(3); break;
		case 64:  setCurrentItem(4); break;
		case 128: setCurrentItem(5); break;
	}
}

/** class ViewSizeDialog: */

ViewSizeDialog::ViewSizeDialog(QWidget *parent, int w, int h)
 : QDialog(parent, "ViewSizeDialog")
{
	QLabel *label = new QLabel(i18n(
		"Resize the window to select the image size\n"
		"and close it or press Escape to accept changes."), this);
	label->move(8, 8);
	label->setFixedSize( label->sizeHint() );

	// setSizeGripEnabled(true) doesn't work (the grip stay at the same place), so we emulate it:
	m_sizeGrip = new QSizeGrip(this);
	m_sizeGrip->setFixedSize( m_sizeGrip->sizeHint() );

	setGeometry(x(), y(), w, h);
}

ViewSizeDialog::~ViewSizeDialog()
{
}

void ViewSizeDialog::resizeEvent(QResizeEvent *)
{
	setCaption( i18n("%1 by %2 pixels").arg(QString::number(width())).arg(QString::number(height())) );
	m_sizeGrip->move( width() - m_sizeGrip->width(), height() - m_sizeGrip->height() );
}

/** class KURLLabel: */

HelpLabel::HelpLabel(const QString &text, const QString &message, QWidget *parent)
 : KURLLabel(parent), m_message(message)
{
	setText(text);
	connect( this, SIGNAL(leftClickedURL()), this, SLOT(showMessage()) );
}

HelpLabel::~HelpLabel()
{
}

void HelpLabel::showMessage()
{
	// TODO: Align it with the label!
	QWhatsThis::display(m_message);
}

/** class IconSizeDialog: */

class UndraggableKIconView : public KIconView
{
  public:
	UndraggableKIconView(QWidget * parent = 0, const char * name = 0, WFlags f = 0) : KIconView(parent, name, f) {}
	QDragObject* dragObject() { return 0; }
};

IconSizeDialog::IconSizeDialog(const QString &caption, const QString &message, const QString &icon, int iconSize, QWidget *parent)
 : KDialogBase(KDialogBase::Swallow, caption, KDialogBase::Ok | KDialogBase::Cancel,
               KDialogBase::Ok, parent, /*name=*/0, /*modal=*/true, /*separator=*/false)
{
	QWidget *page = new QWidget(this);
	QVBoxLayout *topLayout = new QVBoxLayout(page, /*margin=*/0, spacingHint());

	QLabel *label = new QLabel(message, page);
	topLayout->addWidget(label);

	KIconView *iconView = new UndraggableKIconView(page);
	iconView->setItemsMovable(false);
	iconView->setSelectionMode(KIconView::Single);
	m_size16  = new KIconViewItem(iconView, 0,        i18n("16 by 16 pixels"),   DesktopIcon(icon, 16));
	m_size22  = new KIconViewItem(iconView, m_size16, i18n("22 by 22 pixels"),   DesktopIcon(icon, 22));
	m_size32  = new KIconViewItem(iconView, m_size22, i18n("32 by 32 pixels"),   DesktopIcon(icon, 32));
	m_size48  = new KIconViewItem(iconView, m_size32, i18n("48 by 48 pixels"),   DesktopIcon(icon, 48));
	m_size64  = new KIconViewItem(iconView, m_size48, i18n("64 by 64 pixels"),   DesktopIcon(icon, 64));
	m_size128 = new KIconViewItem(iconView, m_size64, i18n("128 by 128 pixels"), DesktopIcon(icon, 128));
	iconView->setMinimumWidth(m_size16->width() + m_size22->width() + m_size32->width() + m_size48->width() + m_size64->width() + m_size128->width() +
	                          (6+2) * iconView->spacing() + 20);
	iconView->setMinimumHeight(m_size128->height() + 2 * iconView->spacing() + 20);
	topLayout->addWidget(iconView);
	switch (iconSize) {
		case 16:  iconView->setSelected(m_size16,  true); m_iconSize = 16;  break;
		case 22:  iconView->setSelected(m_size22,  true); m_iconSize = 22;  break;
		default:
		case 32:  iconView->setSelected(m_size32,  true); m_iconSize = 32;  break;
		case 48:  iconView->setSelected(m_size48,  true); m_iconSize = 48;  break;
		case 64:  iconView->setSelected(m_size64,  true); m_iconSize = 64;  break;
		case 128: iconView->setSelected(m_size128, true); m_iconSize = 128; break;
	}

	connect( iconView, SIGNAL(executed(QIconViewItem*)),      this, SLOT(choose(QIconViewItem*)) );
	connect( iconView, SIGNAL(returnPressed(QIconViewItem*)), this, SLOT(choose(QIconViewItem*)) );
	connect( iconView, SIGNAL(selectionChanged()),            this, SLOT(slotSelectionChanged()) );

	setMainWidget(page);
}

IconSizeDialog::~IconSizeDialog()
{
}

void IconSizeDialog::slotSelectionChanged()
{
	// Change m_iconSize to the new selected one:
	if (m_size16->isSelected())  { m_iconSize = 16;  return; }
	if (m_size22->isSelected())  { m_iconSize = 22;  return; }
	if (m_size32->isSelected())  { m_iconSize = 32;  return; }
	if (m_size48->isSelected())  { m_iconSize = 48;  return; }
	if (m_size64->isSelected())  { m_iconSize = 64;  return; }
	if (m_size128->isSelected()) { m_iconSize = 128; return; }

	// But if user unselected the item (by eg. right clicking a free space), reselect the last one:
	switch (m_iconSize) {
		case 16:  m_size16->setSelected(true);  m_iconSize = 16;  break;
		case 22:  m_size22->setSelected(true);  m_iconSize = 22;  break;
		default:
		case 32:  m_size32->setSelected(true);  m_iconSize = 32;  break;
		case 48:  m_size48->setSelected(true);  m_iconSize = 48;  break;
		case 64:  m_size64->setSelected(true);  m_iconSize = 64;  break;
		case 128: m_size128->setSelected(true); m_iconSize = 128; break;
	}
}

void IconSizeDialog::choose(QIconViewItem*)
{
	actionButton(KDialogBase::Ok)->animateClick();
}

void IconSizeDialog::slotCancel()
{
	m_iconSize = -1;
	KDialogBase::slotCancel();
}

#include "variouswidgets.moc"
