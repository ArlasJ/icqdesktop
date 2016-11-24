#include "stdafx.h"

#include "../../main_window/MainWindow.h"
#include "../../utils/InterConnector.h"
#include "../../utils/utils.h"
#include "../../utils/Text2DocConverter.h"

#include "Common.h"
#include "ContactList.h"
#include "../../gui_settings.h"

namespace ContactList
{
	VisualDataBase::VisualDataBase(
		const QString& _aimId,
		const QPixmap& _avatar,
		const QString& _state,
		const QString& _status,
		const bool _isHovered,
		const bool _isSelected,
		const QString& _contactName,
		const bool _haveLastSeen,
		const QDateTime& _lastSeen,
		bool _isWithCheckBox,
        bool _isChatMember,
        bool _isOfficial,
        const bool _drawLastRead,
        const QPixmap& _lastReadAvatar,
        const QString& _role,
        int _unreadsCounter,
        const QString _term)

		: AimId_(_aimId)
		, Avatar_(_avatar)
		, State_(_state)
		, Status_(_status)
		, IsHovered_(_isHovered)
		, IsSelected_(_isSelected)
		, ContactName_(_contactName)
		, HaveLastSeen_(_haveLastSeen)
		, LastSeen_(_lastSeen)
		, isCheckedBox_(_isWithCheckBox)
        , isChatMember_(_isChatMember)
        , isOfficial_(_isOfficial)
        , drawLastRead_(_drawLastRead)
        , lastReadAvatar_(_lastReadAvatar)
        , role_(_role)
        , unreadsCounter_(_unreadsCounter)
        , searchTerm_(_term)
	{
		assert(!AimId_.isEmpty());
		assert(!ContactName_.isEmpty());
	}

    const QString& VisualDataBase::GetStatus() const
    {
        return Status_;
    }

    bool VisualDataBase::HasStatus() const
    {
        return !Status_.isEmpty();
    }

    void VisualDataBase::SetStatus(const QString& _status)
    {
        Status_ = _status;
    }

	int DipPixels::px() const
	{
		return Utils::scale_value(Px_);
	}

    DipFont::DipFont(const Fonts::FontFamily _family, const Fonts::FontWeight _weight, const DipPixels _size)
		: Family_(_family)
		, Size_(_size)
        , Weight_(_weight)
	{
//		assert(!Family_.isEmpty());
	}

	QFont DipFont::font() const
	{
        return Fonts::appFont(Size_.px(), Family_, Weight_);
	}

	QString FormatTime(const QDateTime& _time)
	{
		if (!_time.isValid())
		{
			return QString();
		}

		const auto current = QDateTime::currentDateTime();

		const auto days = _time.daysTo(current);

		if (days == 0)
		{
			const auto minutes = _time.secsTo(current) / 60;
			if (minutes < 1)
			{
				return QT_TRANSLATE_NOOP("contact_list", "now");
			}

			if (minutes < 10)
			{
				return QVariant(minutes).toString() + QT_TRANSLATE_NOOP("contact_list", " min");
			}

			return _time.time().toString(Qt::SystemLocaleShortDate);
		}

		if (days == 1)
		{
			return QT_TRANSLATE_NOOP("contact_list", "yesterday");
		}

		const auto date = _time.date();
		return Utils::GetTranslator()->formatDate(date, date.year() == current.date().year());
	}

	QTextBrowserUptr CreateTextBrowser(const QString& _name, const QString& _stylesheet, const int _textHeight)
	{
		assert(!_name.isEmpty());
		assert(!_stylesheet.isEmpty());

		QTextBrowserUptr ctrl(new QTextBrowser);

		ctrl->setObjectName(_name);
		ctrl->setStyleSheet(_stylesheet);
        if (_textHeight)
		    ctrl->setFixedHeight(_textHeight);

		ctrl->setFrameStyle(QFrame::NoFrame);
		ctrl->setContentsMargins(0, 0, 0, 0);

		ctrl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		ctrl->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		return ctrl;
	}

	DipPixels ItemWidth(bool _fromAlert, bool _isWithCheckBox, bool _isShortView, bool _isPictureOnlyView)
	{
        if (_isPictureOnlyView)
        {
            // TODO (*) : use RecentsItemRender.cpp
            return DipPixels(56) + DipPixels(2 * 16);
        }

		if (_fromAlert)
			return ContactList::GetRecentsParams(Logic::MembersWidgetRegim::FROM_ALERT).itemWidthAlert();

        if (_isShortView)
            return DipPixels(280);

		// TODO : use L::checkboxWidth
        return (_isWithCheckBox ? dip(30) : dip(0)) + std::min(dip(400), ItemLength(true, 1. / 3, dip(0)));
	}

    DipPixels ItemWidth(const ViewParams& _viewParams)
    {
        return ItemWidth(_viewParams.regim_ == ::Logic::MembersWidgetRegim::FROM_ALERT,
            _viewParams.regim_ == Logic::MembersWidgetRegim::SELECT_MEMBERS, _viewParams.shortView_, _viewParams.pictOnly_);
    }

    int CorrectItemWidth(int _itemWidth, int _fixedWidth)
    {
        return _fixedWidth == -1 ? _itemWidth : _fixedWidth;
    }

	DipPixels ItemLength(bool _isWidth, double _koeff, DipPixels _addWidth)
	{
        assert(!!Utils::InterConnector::instance().getMainWindow() && "Common.cpp (ItemLength)");
        auto mainRect = Utils::GetMainRect();
        if (mainRect.width() && mainRect.height())
        {
            auto mainLength = _isWidth ? mainRect.width() : mainRect.height();
            return _addWidth + DipPixels(Utils::unscale_value(mainLength) * _koeff);
        }
        assert("Couldn't get rect: Common.cpp (ItemLength)");
        return DipPixels(0);
	}

    int ContactItemHeight()
    {
        return Utils::scale_value(44);
    }

    int SearchInAllChatsHeight()
    {
        return Utils::scale_value(49);
    }

    int GroupItemHeight()
    {
        return Utils::scale_value(28);
    }

    bool IsPictureOnlyView()
    {
        auto mainRect = Utils::GetMainRect();

        // TODO : use Utils::MinWidthForMainWindow
        return mainRect.width() <= dip(800).px();
    }

    const QString ContactListParams::getRecentsNameFontColor(const bool isUnread)
    {
        const auto color =
            isUnread ?
            Utils::rgbaStringFromColor(Ui::CommonStyle::getTextCommonColor()):
            Utils::rgbaStringFromColor(Ui::CommonStyle::getTextCommonColor());

        return color;
    };

    const QString ContactListParams::getRecentsMessageFontColor(const bool isUnread)
    {
        const auto color = (isUnread ? Utils::rgbaStringFromColor(Ui::CommonStyle::getTextCommonColor()) : "#696969");

        return color;
    };

    const QString ContactListParams::getRecentsFontWeight(const bool isUnread)
    {
        const auto fontWeight = Fonts::appFontWeightQss(
            isUnread ?
                Fonts::FontWeight::Semibold:
                Fonts::FontWeight::Normal);

        return fontWeight;
    };

    ContactListParams& GetContactListParams()
    {
        static ContactListParams params(true);
        return params;
    }

    ContactListParams& GetRecentsParams(int _regim)
    {
        if (_regim == Logic::MembersWidgetRegim::FROM_ALERT || _regim == Logic::MembersWidgetRegim::HISTORY_SEARCH)
        {
            static ContactListParams params(false);
            return params;
        }
        else
        {
            const auto show_last_message = Ui::get_gui_settings()->get_value<bool>(settings_show_last_message, true);
            static ContactListParams params(!show_last_message);
            params.setIsCL(!show_last_message);
            return params;
        }
    }

    void RenderAvatar(QPainter &painter, int _x, const QPixmap& _avatar, ContactListParams& _contactListPx)
    {
        if (_avatar.isNull())
        {
            return;
        }

        painter.drawPixmap(_x, _contactListPx.avatarY().px(), _contactListPx.avatarW().px(), _contactListPx.avatarH().px(), _avatar);
    }

    void RenderMouseState(QPainter &painter, const bool _isHovered, const bool _isSelected, const ContactListParams& _contactListPx, const ViewParams& viewParams_)
    {
        if (!_isHovered && !_isSelected)
        {
            return;
        }

        painter.save();

        if (_isHovered)
        {
            static QBrush hoverBrush(Ui::CommonStyle::getContactListHoveredColor());
            painter.setBrush(hoverBrush);
        }
        if (_isSelected)
        {
            static QBrush selectedBrush(Ui::CommonStyle::getContactListSelectedColor());
            painter.setBrush(selectedBrush);
        }

        auto width = CorrectItemWidth(ItemWidth(viewParams_).px(), viewParams_.fixedWidth_);
        painter.drawRect(0, 0, width, _contactListPx.itemHeight().px());

        painter.restore();
    }

    int RenderDate(QPainter &painter, const QDateTime &ts, const VisualDataBase &item, ContactListParams& _contactListPx, const ViewParams& _viewParams)
    {
        const auto regim = _viewParams.regim_;
        const auto isWithCheckBox = regim == Logic::MembersWidgetRegim::SELECT_MEMBERS;
        auto timeXRight = CorrectItemWidth(ItemWidth(_viewParams).px() - _contactListPx.itemPadding().px(), _viewParams.fixedWidth_)
            - _viewParams.rightMargin_ - _contactListPx.itemPadding().px();

        if (_viewParams.regim_ == ::Logic::MembersWidgetRegim::HISTORY_SEARCH)
        {
            timeXRight = CorrectItemWidth(ItemWidth(_viewParams).px(), _viewParams.fixedWidth_) - _viewParams.rightMargin_ - _contactListPx.itemPadding().px();
        }

        if (!ts.isValid())
        {
            return timeXRight;
        }

        const auto timeStr = FormatTime(ts);
        if (timeStr.isEmpty())
        {
            return timeXRight;
        }

        static QFontMetrics m(_contactListPx.timeFont().font());
        const auto leftBearing = m.leftBearing(timeStr[0]);
        const auto rightBearing = m.rightBearing(timeStr[timeStr.length() - 1]);
        const auto timeWidth = (m.tightBoundingRect(timeStr).width() + leftBearing + rightBearing);
        const auto timeX = timeXRight - timeWidth;

        if ((!isWithCheckBox && !Logic::is_delete_members_regim(regim) && !Logic::is_admin_members_regim(regim))
            || (Logic::is_delete_members_regim(regim) && !item.IsHovered_)
            || (!Logic::is_admin_members_regim(regim) && !item.IsHovered_))
        {
            painter.save();
            painter.setFont(_contactListPx.timeFont().font());
            painter.setPen(_contactListPx.timeFontColor());
            painter.drawText(timeX, _contactListPx.timeY().px(), timeStr);
            painter.restore();
        }

        return timeX;
    }

    void RenderContactName(QPainter &painter, const ContactList::VisualDataBase &visData, const int y, const int rightBorderPx, ContactList::ViewParams _viewParams, ContactList::ContactListParams& _contactListPx)
    {
        assert(y > 0);
        assert(rightBorderPx > 0);
        assert(!visData.ContactName_.isEmpty());

        _contactListPx.setLeftMargin(_viewParams.leftMargin_);

        QString color;
        auto weight = Fonts::FontWeight::Normal;
        QString name;
        int height = 0;
        if (_contactListPx.isCL())
        {
            color = _viewParams.regim_ == Logic::MembersWidgetRegim::SELECT_MEMBERS
                && (visData.isChatMember_ || visData.isCheckedBox_) ? "#579e1c": Utils::rgbaStringFromColor(Ui::CommonStyle::getTextCommonColor());
            name = "name";
            height = _contactListPx.contactNameHeight().px();
        }
        else
        {
            const auto hasUnreads = (_viewParams.regim_ != ::Logic::MembersWidgetRegim::FROM_ALERT && (visData.unreadsCounter_ > 0));
            weight = (hasUnreads ? Fonts::FontWeight::Medium : Fonts::FontWeight::Normal);
            color = _contactListPx.getNameFontColor(hasUnreads);
            name = hasUnreads ? "nameUnread" : "name";
            height = _contactListPx.contactNameHeight().px() + (platform::is_apple() ? 1 : 0);
        }

        const auto styleSheetQss = _contactListPx.getContactNameStylesheet(color, weight);

        static auto textControl = CreateTextBrowser(
            name,
            styleSheetQss,
            height);

        textControl.get()->setStyleSheet(styleSheetQss);

        QPixmap official_mark;
        if (visData.isOfficial_)
        {
            official_mark = QPixmap(Utils::parse_image_name(":/resources/cl_badges_official_100.png"));
            Utils::check_pixel_ratio(official_mark);
        }

        int maxWidth = rightBorderPx - _contactListPx.GetContactNameX().px();
        if (_contactListPx.isCL())
        {
            maxWidth -= _contactListPx.contactNameRightPadding().px();
        }

        if (!official_mark.isNull())
            maxWidth -= official_mark.width();

        textControl->setFixedWidth(maxWidth);

        QFontMetrics m(textControl->font());
        const auto elidedString = m.elidedText(visData.ContactName_, Qt::ElideRight, maxWidth);

        auto &doc = *textControl->document();
        doc.clear();

        QTextCursor cursor = textControl->textCursor();
        Logic::Text2Doc(elidedString, cursor, Logic::Text2DocHtmlMode::Pass, false);
        Logic::FormatDocument(doc, _contactListPx.contactNameHeight().px());

        qreal correction = 0;
        if (platform::is_apple())
        {
            qreal realHeight = doc.documentLayout()->documentSize().toSize().height();

            if (_contactListPx.isCL())
                correction = (realHeight > 20) ? 0 : 2;
            else
                correction = (realHeight > 21) ? -2 : 2;
            textControl->render(&painter, QPoint(_contactListPx.GetContactNameX().px(), y + correction));
        }
        else
        {
            textControl->render(&painter, QPoint(_contactListPx.GetContactNameX().px(), y));
        }

        int pX = _contactListPx.GetContactNameX().px() + m.width(elidedString) + _contactListPx.official_hor_padding().px();
        int pY = y + _contactListPx.official_ver_padding().px();
        painter.drawPixmap(pX, pY, official_mark);
    }

    int RenderRemove(QPainter &painter, ContactListParams& _contactListPx, const ViewParams& _viewParams)
    {
        const auto _shortView = _viewParams.shortView_;
        const auto width = _viewParams.fixedWidth_;
        const auto remove_img = Utils::parse_image_name(":/resources/contr_clear_100.png");
        const auto isWithCheckBox = false;
        painter.save();

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        painter.drawPixmap(GetXOfRemoveImg(isWithCheckBox, _shortView, width)
            , _contactListPx.itemHeight().px() / 2 - _contactListPx.remove_size().px() / 2
            , _contactListPx.remove_size().px()
            , _contactListPx.remove_size().px()
            , remove_img);

        painter.restore();

        const auto xPos = CorrectItemWidth(ItemWidth(_viewParams).px(), width) - _contactListPx.itemPadding().px() - _contactListPx.remove_size().px() - _contactListPx.onlineSignLeftPadding().px();
        assert(xPos > _contactListPx.itemLeftBorder().px());
        return xPos;
    }

    int GetXOfRemoveImg(bool _isWithCheckBox, bool _shortView, int width)
    {
        auto contactListPx = GetContactListParams();
        return CorrectItemWidth(ItemWidth(false, _isWithCheckBox, _shortView).px(), width) - contactListPx.itemPadding().px() - DipPixels(contactListPx.onlineSignSize() + contactListPx.remove_size()).px();
    }
}
