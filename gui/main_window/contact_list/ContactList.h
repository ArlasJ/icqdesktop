#pragma once

#include "../../types/typing.h"
#include "../../controls/TransparentScrollBar.h"
#include "Common.h"

namespace Logic
{
    class UnknownItemDelegate;
	class RecentItemDelegate;
	class ContactListItemDelegate;
    class ChatMembersModel;
    class AbstractSearchModel;
    class LiveChatItemDelegate;

	enum MembersWidgetRegim
    {
        CONTACT_LIST,
        SELECT_MEMBERS,
        DELETE_MEMBERS,
        IGNORE_LIST,
        ADMIN_MEMBERS,
        SHARE_LINK,
        SHARE_TEXT,
        PENDING_MEMBERS,
        UNKNOWN,
        FROM_ALERT,
        HISTORY_SEARCH,
    };

    bool is_delete_members_regim(int _regim);
    bool is_admin_members_regim(int _regim);
}

namespace Data
{
	class Contact;
}

namespace Utils
{
    class SignalsDisconnector;
}

namespace Ui
{
    class SettingsTab;
    class ContextMenu;
    class ContactList;

    class AddContactButton : public QWidget
    {
        Q_OBJECT
    Q_SIGNALS:
        void clicked();
    public:
        AddContactButton(QWidget* _parent);

    protected:
        virtual void paintEvent(QPaintEvent*);
        virtual void enterEvent(QEvent *);
        virtual void leaveEvent(QEvent *);
        virtual void mousePressEvent(QMouseEvent *);
        virtual void mouseReleaseEvent(QMouseEvent *);

    private:
        QPainter* painter_;
        bool hover_;
        bool select_;
    };

    class SearchInAllChatsButton: public QWidget
    {
        Q_OBJECT
    Q_SIGNALS:
        void clicked();
    public:
        SearchInAllChatsButton(QWidget* _parent = nullptr);

    protected:
        virtual void paintEvent(QPaintEvent*);
        virtual void enterEvent(QEvent *);
        virtual void leaveEvent(QEvent *);
        virtual void mousePressEvent(QMouseEvent *);
        virtual void mouseReleaseEvent(QMouseEvent *);

    private:
        QPainter* painter_;
        bool hover_;
        bool select_;
    };

    class SearchInChatLabel: public QWidget
    {
        Q_OBJECT

    public:
        SearchInChatLabel(QWidget* _parent = nullptr);

    protected:
        virtual void paintEvent(QPaintEvent*);

    private:
        QPainter* painter_;
    };

    class EmptyIgnoreListLabel : public QWidget
    {
        Q_OBJECT
    public:
        EmptyIgnoreListLabel(QWidget* _parent);

    protected:
        virtual void paintEvent(QPaintEvent*);

    private:
        QPainter* painter_;
    };

    class RCLEventFilter : public QObject
    {
        Q_OBJECT

    public:
        RCLEventFilter(ContactList* _cl);
    protected:
        bool eventFilter(QObject* _obj, QEvent* _event);
    private:
        ContactList* cl_;
    };

	enum CurrentTab
	{
		RECENTS = 0,
		ALL,
        LIVE_CHATS,
		SETTINGS,
		SEARCH,
	};

	class RecentsButton : public QPushButton
	{
		Q_OBJECT
	public:
		RecentsButton(QWidget* _parent);

	protected:
		void paintEvent(QPaintEvent *);

	private:
		QPainter* painter_;
	};

	class ContactList : public QWidget
	{
		Q_OBJECT

	Q_SIGNALS:
		void itemSelected(QString, qint64 _message_id);
        void itemClicked(QString);
		void groupClicked(int);
		void searchEnd();
		void addContactClicked();
        void needSwitchToRecents();

	public Q_SLOTS:
		void searchResult();
		void searchUpPressed();
		void searchDownPressed();
        void onSendMessage(QString);
        void select(QString, qint64 _message_id = -1);

        void changeSelected(QString _aimId, bool _isRecent);
		void recentsClicked();
        void liveChatsClicked();
		void allClicked();
		void settingsClicked();
        void switchToRecents();
        void onDisableSearchInDialogButton();

        void setSearchInDialog(bool _isSearchInDialog);
        bool getSearchInDialog() const;

	private Q_SLOTS:
		void searchResults(const QModelIndex &, const QModelIndex &);
        void searchResultsFromModel();
		void itemClicked(const QModelIndex&);
		void itemPressed(const QModelIndex&);
        void liveChatsItemPressed(const QModelIndex&);
		void statsRecentItemPressed(const QModelIndex&);
		void statsSearchItemPressed(const QModelIndex&);
		void statsCLItemPressed(const QModelIndex&);
		void searchClicked(const QModelIndex&);

        void guiSettingsChanged();
		void recentOrderChanged();
		void touchScrollStateChangedRecents(QScroller::State);
		void touchScrollStateChangedCl(QScroller::State);
        void touchScrollStateChangedSearch(QScroller::State);
        void touchScrollStateChangedLC(QScroller::State);

        void showNoContactsYet();
        void hideNoContactsYet();

        void showNoRecentsYet();
        void hideNoRecentsYet();

        void showNoSearchResults();
        void hideNoSearchResults();

        void showSearchSpinner();
        void hideSearchSpinner();

        void typingStatus(Logic::TypingFires _typing, bool _isTyping);

        void messagesReceived(QString, QVector<QString>);
		void showPopupMenu(QAction* _action);
        void switchTab(QString);
        void autoScroll();

        void dialogClosed(QString _aimid);


	public:

		ContactList(QWidget* _parent, Logic::MembersWidgetRegim _regim, Logic::ChatMembersModel* _chatMembersModel);
		~ContactList();

		void setSearchMode(bool);
		bool isSearchMode() const;
		bool isContactListMode() const;
        bool shouldHideSearch() const;
		void changeTab(CurrentTab _currTab);
        inline CurrentTab currentTab() const
        {
            return (CurrentTab)currentTab_;
        }
        void triggerTapAndHold(bool _value);
        bool tapAndHoldModifier() const;
        void dragPositionUpdate(const QPoint& _pos, bool fromScroll = false);
        void dropFiles(const QPoint& _pos, const QList<QUrl> _files);
        void showContactList();
        void setEmptyIgnoreLabelVisible(bool _isVisible);
        void setClDelegate(Logic::ContactListItemDelegate* _delegate);
        void setTransparent(bool transparent);
        void clearSettingsSelection();
        void selectSettingsVoipTab();

        void setPictureOnlyView(bool _isPictureOnly);
        bool getPictureOnlyView() const;
        void setButtonsVisibility(bool _isShow);
        void setItemWidth(int _newWidth);

        QString getSelectedAimid() const;

	private:

		void updateTabState(bool _save);
        void updateCheckedButtons();
        void updateSettingsState();
		void showRecentsPopup_menu(const QModelIndex& _current);
		void showContactsPopupMenu(QString _aimid, bool _is_chat);
		void selectionChanged(const QModelIndex &);

        QString getAimid(const QModelIndex &_current) const;
        void searchUpOrDownPressed(bool _isUp);

        void showNoRecentsYet(QWidget *_parent, QWidget *_list, QLayout *_layout, std::function<void()> _action);
        void hideNoRecentsYet(QWidget *_list, QLayout *_layout);

        void showNoContactsYet(QWidget *_parent, QWidget *_list, QLayout *_layout);
        void hideNoContactsYet(QWidget *_list, QLayout *_layout);

        void showNoSearchResults(QWidget *_parent, QWidget *_list, QLayout *_layout);
        void hideNoSearchResults(QWidget *_list, QLayout *_layout);

        void showSearchSpinner(QWidget *_parent, QWidget *_list, QLayout *_layout);
        void hideSearchSpinner(QWidget *_list, QLayout *_layout);

    private:
        Logic::ChatMembersModel*						chatMembersModel_;
		Logic::ContactListItemDelegate*					clDelegate_;
        Logic::LiveChatItemDelegate*                    liveChatsDelegate_;
		QPushButton*									clTabButton_;
        QVBoxLayout*									contactListLayout_;
		QWidget*										contactListPage_;
		FocusableListView*								contactListView_;
		EmptyIgnoreListLabel*							emptyIgnoreListLabel_;
		RCLEventFilter*									listEventFilter_;
		QPushButton*									livechatsButton_;
		QWidget*										liveChatsPage_;
		FocusableListView*								liveChatsView_;
		QWidget*										noContactsYet_;
		QWidget*										noRecentsYet_;
		QWidget*										noSearchResults_;
		QWidget*										searchSpinner_;
		Ui::ContextMenu*								popupMenu_;
		Logic::MembersWidgetRegim						regim_;
		RecentsButton*									recentsButton_;
		Logic::RecentItemDelegate*						recentsDelegate_;
        Logic::UnknownItemDelegate                      *unknownsDelegate_;
        Logic::AbstractItemDelegateWithRegim*           searchItemDelegate_;

		QVBoxLayout*									recentsLayout_;
		QWidget*										recentsPage_;
		FocusableListView*								recentsView_;

        QVBoxLayout*									searchLayout_;
		QWidget*										searchPage_;
        FocusableListView*								searchView_;

        QString                                         lastSearchPattern_;
		
        SettingsTab*									settingsTab_;
		QPushButton*									settingsTabButton_;
		QStackedWidget*									stackedWidget_;
        QFrame*                                         buttonsFrame_;
        QTimer*                                         scrollTimer_;
        FocusableListView*                              scrolledView_;
        int                                             scrollMultipler_;
        QPoint                                          lastDragPos_;
        SearchInAllChatsButton*                         searchInAllButton_;
        SearchInChatLabel*                              searchInChatLabel_;

        unsigned										currentTab_;
        bool											noContactsYetShown_;
        bool											noRecentsYetShown_;
        bool											noSearchResultsShown_;
        bool											searchSpinnerShown_;
        bool											tapAndHold_;
        bool                                            pictureOnlyView_;
        int                                             fixedItemWidth_;
        bool                                            isSearchInDialog_;
	};
}
