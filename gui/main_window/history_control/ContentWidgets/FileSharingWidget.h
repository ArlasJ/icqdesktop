#pragma once

#include "../../../namespaces.h"

#include "PreviewContentWidget.h"

UI_NS_BEGIN

class ActionButtonWidget;

UI_NS_END

THEMES_NS_BEGIN

typedef std::shared_ptr<class IThemePixmap> IThemePixmapSptr;

THEMES_NS_END

namespace Ui
{
    class MessageItem;
}

namespace HistoryControl
{

	typedef std::shared_ptr<class FileSharingInfo> FileSharingInfoSptr;

	class FileSharingWidget : public PreviewContentWidget
	{
		Q_OBJECT

	// --------------------------------------------------------------------------------------------------------------------------------
	// Qt Properties

	public:
		Q_PROPERTY(int DownloadingBarBaseAngle READ getDownloadingBarBaseAngle WRITE setDownloadingBarBaseAngle)

		void setDownloadingBarBaseAngle(int _val);

		int getDownloadingBarBaseAngle() const;

	// --------------------------------------------------------------------------------------------------------------------------------
	// Public

	public:
        FileSharingWidget(const FileSharingInfoSptr& fsInfo, const QString& contactUin);

		FileSharingWidget(
            Ui::MessageItem* parent,
            const bool isOutgoing,
            const QString &contactUin,
            const FileSharingInfoSptr& fsInfo,
            const bool previewsEnabled);

		virtual ~FileSharingWidget() override;

		virtual bool isBlockElement() const override;

        virtual bool canReplace() const override;

		virtual bool canUnload() const override;

		virtual QString toLogString() const override;

        virtual QString toRecentsString() const override;

		virtual QString toString() const override;

        virtual void copyFile() override;

        virtual void saveAs() override;

        virtual bool haveContentMenu(QPoint) const override;

        virtual QString toLink() const override;

        virtual bool haveOpenInBrowserMenu() override;

	// --------------------------------------------------------------------------------------------------------------------------------
	// Protected

	protected:
        virtual void enterEvent(QEvent *event) override;

        virtual void initialize() override;

        virtual bool isPreloaderVisible() const override;

        virtual void leaveEvent(QEvent *event) override;

		virtual void mouseMoveEvent(QMouseEvent *event) override;

		virtual void mouseReleaseEvent(QMouseEvent *event) override;

        virtual void renderPreview(QPainter &p, const bool isAnimating) override;

        virtual void resizeEvent(QResizeEvent *e) override;

        virtual bool drag() override;

	// --------------------------------------------------------------------------------------------------------------------------------
	// Slots

	private Q_SLOTS:
		void onFileMetainfo(qint64 seq, QString filename, QString downloadUri, qint64 size);

        void onPreviewMetainfo(qint64 seq, QString miniPreviewUri, QString fullPreviewUri);

		void imageDownloaded(qint64, QString, QPixmap, QString);

		void fileDownloaded(qint64, QString, QString);

		void fileDownloading(qint64, QString, qint64);

		void fileSharingError(qint64 seq, QString rawUri, qint32 errorCode);

		void fileLocalCopyChecked(qint64, bool, QString);

		void fileSharingUploadingProgress(QString, qint64);

        void fileSharingUploadingResult(QString seq, bool success, QString localPath, QString uri, int contentType, bool isFileTooBig);

        void localPreviewLoaded(QPixmap pixmap);

        void previewDownloaded(qint64, QString, QPixmap, QString);

        void previewDownloadError(int64_t seq, QString rawUri);

	// --------------------------------------------------------------------------------------------------------------------------------
	// Private Member Variables

	private:
        Ui::MessageItem* ParentItem_;

        enum class State;

        enum class PreviewState;

		struct
		{
			mutable QRect ControlButtonPreviewRect_;

			mutable QRect ControlButtonPlainRect_;

			State State_;
		} Private_; // can't touch this

		struct
		{
			QString MiniPreviewUri_;

			QString FullPreviewUri_;

			QString Filename_;

			qint64 FileSize_;

			QString FileSizeStr_;

			QString DownloadUri_;
		} Metainfo_;

		const FileSharingInfoSptr FsInfo_;

		struct
		{
			QPixmap FullImg_;
		} Preview_;

		qint64 BytesTransferred_;

		Themes::IThemePixmapSptr FileTypeIcon_;

		QString FileSizeAndProgressStr_;

		QRect OpenDownloadsDirButtonRect_;

		QString DownloadedFileLocalPath_;

		qint32 DownloadingBarBaseAngle_;

		QPropertyAnimation *BaseAngleAnimation_;

        bool SaveAs_;

        bool CopyFile_;

        PreviewState PreviewState_;

        int64_t FileMetainfoDownloadId_;

        int64_t PreviewMetainfoDownloadId_;

        int64_t FileDownloadId_;

        int64_t PreviewDownloadId_;

        int64_t CheckLocalCopyExistenceId_;

        QString LastProgressText_;

        QRect ProgressTextRect_;

        Themes::IThemePixmapSptr CurrentCtrlIcon_;

        bool IsCtrlButtonHovered_;

        struct Retry
        {
            Retry();

            bool HasRetryFlagSet() const;

            bool ShouldRetry() const;

            bool ShouldRetryFileDownload() const;

            bool ShouldRetryFileMetainfo() const;

            bool ShouldRetryPreviewDownload() const;

            bool FileMetainfo_;

            int FileMetainfoRetryCount_;

            bool FileDownload_;

            int FileDownloadRetryCount_;

            bool PreviewDownload_;

            int PreviewDownloadRetryCount_;
        } Retry_;

        QSharedPointer<QMovie> GifImage_;

        Ui::ActionButtonWidget *ShareButton_;

	// --------------------------------------------------------------------------------------------------------------------------------
	// Private Methods

	private:
		bool canStartImageDownloading(const QPoint &mousePos) const;

		void checkLocalCopyExistence();

        void connectErrorSignal();

		void connectFileDownloadSignals();

        void connectFileUploadingSignals();

		void connectMetainfoSignal();

        void connectPreviewSignals();

        void connectSignals();

		void convertToPlainFileView();

		void convertToUploadErrorView();

        QString elideFilename(const QString &text, const QFont &font, const int32_t maxTextWidth);

        void formatFileSizeStr();

		const QRect& getControlButtonRect(const QSize &iconSize) const;

		const QRect& getControlButtonPlainRect(const QSize &iconSize) const;

		const QRect& getControlButtonPreviewRect(const QSize &iconSize) const;

		bool getLocalFileMetainfo();

		State getState() const;

        void initializeShareButton();

        bool isControlButtonVisible() const;

		bool isDataTransferProgressVisible() const;

        bool isFullImageDownloading() const;

        bool isGifImage() const;

        bool isGifPlaying() const;

        bool isImagePreview() const;

		bool isFilenameAndSizeVisible() const;

		bool isOpenDownloadsDirButtonVisible() const;

		bool isOverControlButton(const QPoint &p) const;

		bool isOverOpenDownloadsDirButton(const QPoint &p) const;

        bool isOverPreview(const QPoint &p) const;

		bool isPreviewVisible() const;

		bool isState(const State state) const;

        void loadGifImage(const QString &path);

		bool loadPreviewFromLocalFile();

        void onGifFrameUpdated(int frameNumber);

        void onGifImageClicked();

        void onShareButtonClicked();

		void openDownloadsDir() const;

		void renderControlButton(QPainter &p);

		void renderDataTransferProgress(QPainter &p);

        void renderDataTransferProgressText(QPainter &p);

		void renderFilename(QPainter &p);

		void renderFileSizeAndProgress(QPainter &p);

		void renderOpenDownloadsDirButton(QPainter &p);

		void requestFileMetainfo();

		void requestPreview();

        void requestPreviewMetainfo();

		void resumeUploading();

        void retryRequest();

        bool retryRequestLater();

        void setBlockSizePolicy();

		void setInitialWidgetSizeAndState();

        void setState(const State state);

        void showPreviewer(const QPoint &globalPos);

		void startDataTransferAnimation();

		void stopDataTransferAnimation();

		void startDownloadingPlainFile();

		void startDownloadingFullImage();

		void stopDownloading();

		void stopUploading();

        void updateShareButtonGeometry();

	};

}