# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'playmp4page.ui'
##
## Created by: Qt User Interface Compiler version 6.8.1
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QFrame, QHBoxLayout, QLabel,
    QPushButton, QSizePolicy, QSpacerItem, QSpinBox,
    QVBoxLayout, QWidget)

from displaymp4nodelist import DisplayMP4NodeList

class Ui_PlayMP4Page(object):
    def setupUi(self, PlayMP4Page):
        if not PlayMP4Page.objectName():
            PlayMP4Page.setObjectName(u"PlayMP4Page")
        PlayMP4Page.resize(646, 378)
        self.verticalLayout = QVBoxLayout(PlayMP4Page)
        self.verticalLayout.setSpacing(0)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 0, 10)
        self.frame_2 = QFrame(PlayMP4Page)
        self.frame_2.setObjectName(u"frame_2")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(1)
        sizePolicy.setHeightForWidth(self.frame_2.sizePolicy().hasHeightForWidth())
        self.frame_2.setSizePolicy(sizePolicy)
        self.frame_2.setFrameShape(QFrame.Shape.NoFrame)
        self.frame_2.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_4 = QHBoxLayout(self.frame_2)
        self.horizontalLayout_4.setSpacing(0)
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.horizontalLayout_4.setContentsMargins(0, 0, 0, 0)
        self.ShowMVOrLrcArea = QFrame(self.frame_2)
        self.ShowMVOrLrcArea.setObjectName(u"ShowMVOrLrcArea")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy1.setHorizontalStretch(7)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.ShowMVOrLrcArea.sizePolicy().hasHeightForWidth())
        self.ShowMVOrLrcArea.setSizePolicy(sizePolicy1)
        self.ShowMVOrLrcArea.setStyleSheet(u"background : transparent;")
        self.ShowMVOrLrcArea.setFrameShape(QFrame.Shape.NoFrame)
        self.ShowMVOrLrcArea.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_2 = QVBoxLayout(self.ShowMVOrLrcArea)
        self.verticalLayout_2.setSpacing(0)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.verticalLayout_2.setContentsMargins(0, 0, 0, 0)

        self.horizontalLayout_4.addWidget(self.ShowMVOrLrcArea)

        self.display_mp4_node_list = DisplayMP4NodeList(self.frame_2)
        self.display_mp4_node_list.setObjectName(u"display_mp4_node_list")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        sizePolicy2.setHorizontalStretch(3)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.display_mp4_node_list.sizePolicy().hasHeightForWidth())
        self.display_mp4_node_list.setSizePolicy(sizePolicy2)
        self.display_mp4_node_list.setStyleSheet(u"#display_mp4_node_list{\n"
"background:	#696969;\n"
"}\n"
"QObject{\n"
"background : transparent;\n"
"color : white;\n"
"}\n"
"QPushButton{\n"
"background : transparent;\n"
"border : none;\n"
"}\n"
"QPushButton:hover{\n"
"background-color :	#778899;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color :	 #D3D3D3;\n"
"}\n"
"")

        self.horizontalLayout_4.addWidget(self.display_mp4_node_list)


        self.verticalLayout.addWidget(self.frame_2)

        self.frame_4 = QFrame(PlayMP4Page)
        self.frame_4.setObjectName(u"frame_4")
        self.frame_4.setFrameShape(QFrame.Shape.NoFrame)
        self.frame_4.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_2 = QHBoxLayout(self.frame_4)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.ShowLrc = QPushButton(self.frame_4)
        self.ShowLrc.setObjectName(u"ShowLrc")

        self.horizontalLayout_2.addWidget(self.ShowLrc)

        self.ShowMV = QPushButton(self.frame_4)
        self.ShowMV.setObjectName(u"ShowMV")

        self.horizontalLayout_2.addWidget(self.ShowMV)

        self.EditMVOrMP3 = QPushButton(self.frame_4)
        self.EditMVOrMP3.setObjectName(u"EditMVOrMP3")

        self.horizontalLayout_2.addWidget(self.EditMVOrMP3)

        self.EditLrc = QPushButton(self.frame_4)
        self.EditLrc.setObjectName(u"EditLrc")

        self.horizontalLayout_2.addWidget(self.EditLrc)

        self.FullScreenInNMP3 = QPushButton(self.frame_4)
        self.FullScreenInNMP3.setObjectName(u"FullScreenInNMP3")

        self.horizontalLayout_2.addWidget(self.FullScreenInNMP3)

        self.FullSreen = QPushButton(self.frame_4)
        self.FullSreen.setObjectName(u"FullSreen")

        self.horizontalLayout_2.addWidget(self.FullSreen)

        self.ClearNowPlayingSongList = QPushButton(self.frame_4)
        self.ClearNowPlayingSongList.setObjectName(u"ClearNowPlayingSongList")

        self.horizontalLayout_2.addWidget(self.ClearNowPlayingSongList)


        self.verticalLayout.addWidget(self.frame_4)

        self.SimpleEditSongFrame = QFrame(PlayMP4Page)
        self.SimpleEditSongFrame.setObjectName(u"SimpleEditSongFrame")
        self.SimpleEditSongFrame.setFrameShape(QFrame.Shape.StyledPanel)
        self.SimpleEditSongFrame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_3 = QVBoxLayout(self.SimpleEditSongFrame)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.frame_3 = QFrame(self.SimpleEditSongFrame)
        self.frame_3.setObjectName(u"frame_3")
        self.frame_3.setFrameShape(QFrame.Shape.NoFrame)
        self.frame_3.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout = QHBoxLayout(self.frame_3)
        self.horizontalLayout.setSpacing(0)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.frame_5 = QFrame(self.frame_3)
        self.frame_5.setObjectName(u"frame_5")
        self.frame_5.setFrameShape(QFrame.Shape.NoFrame)
        self.frame_5.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_3 = QHBoxLayout(self.frame_5)
        self.horizontalLayout_3.setSpacing(0)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.horizontalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.label = QLabel(self.frame_5)
        self.label.setObjectName(u"label")
        self.label.setAlignment(Qt.AlignmentFlag.AlignRight|Qt.AlignmentFlag.AlignTrailing|Qt.AlignmentFlag.AlignVCenter)

        self.horizontalLayout_3.addWidget(self.label)

        self.InputPercentageOfAdjustTheSongVolumn = QSpinBox(self.frame_5)
        self.InputPercentageOfAdjustTheSongVolumn.setObjectName(u"InputPercentageOfAdjustTheSongVolumn")

        self.horizontalLayout_3.addWidget(self.InputPercentageOfAdjustTheSongVolumn)

        self.label_3 = QLabel(self.frame_5)
        self.label_3.setObjectName(u"label_3")

        self.horizontalLayout_3.addWidget(self.label_3)

        self.ConfirmAdjustSongVolumeByPercentage = QPushButton(self.frame_5)
        self.ConfirmAdjustSongVolumeByPercentage.setObjectName(u"ConfirmAdjustSongVolumeByPercentage")

        self.horizontalLayout_3.addWidget(self.ConfirmAdjustSongVolumeByPercentage)


        self.horizontalLayout.addWidget(self.frame_5)

        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)

        self.horizontalLayout.addItem(self.horizontalSpacer)

        self.frame_6 = QFrame(self.frame_3)
        self.frame_6.setObjectName(u"frame_6")
        self.frame_6.setFrameShape(QFrame.Shape.NoFrame)
        self.frame_6.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_5 = QHBoxLayout(self.frame_6)
        self.horizontalLayout_5.setSpacing(0)
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.horizontalLayout_5.setContentsMargins(0, 0, 0, 0)
        self.label_2 = QLabel(self.frame_6)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setAlignment(Qt.AlignmentFlag.AlignRight|Qt.AlignmentFlag.AlignTrailing|Qt.AlignmentFlag.AlignVCenter)

        self.horizontalLayout_5.addWidget(self.label_2)

        self.Input_dBOfAdjustTheSongVolumn = QSpinBox(self.frame_6)
        self.Input_dBOfAdjustTheSongVolumn.setObjectName(u"Input_dBOfAdjustTheSongVolumn")

        self.horizontalLayout_5.addWidget(self.Input_dBOfAdjustTheSongVolumn)

        self.label_4 = QLabel(self.frame_6)
        self.label_4.setObjectName(u"label_4")

        self.horizontalLayout_5.addWidget(self.label_4)

        self.ConfirmAdjustSongVolumeBy_dB = QPushButton(self.frame_6)
        self.ConfirmAdjustSongVolumeBy_dB.setObjectName(u"ConfirmAdjustSongVolumeBy_dB")

        self.horizontalLayout_5.addWidget(self.ConfirmAdjustSongVolumeBy_dB)


        self.horizontalLayout.addWidget(self.frame_6)


        self.verticalLayout_3.addWidget(self.frame_3)


        self.verticalLayout.addWidget(self.SimpleEditSongFrame)


        self.retranslateUi(PlayMP4Page)

        QMetaObject.connectSlotsByName(PlayMP4Page)
    # setupUi

    def retranslateUi(self, PlayMP4Page):
        PlayMP4Page.setWindowTitle(QCoreApplication.translate("PlayMP4Page", u"Form", None))
        self.ShowLrc.setText(QCoreApplication.translate("PlayMP4Page", u"\u663e\u793a\u6b4c\u8bcd", None))
        self.ShowMV.setText(QCoreApplication.translate("PlayMP4Page", u"\u663e\u793amv", None))
        self.EditMVOrMP3.setText(QCoreApplication.translate("PlayMP4Page", u"\u7f16\u8f91mv/\u6b4c\u66f2", None))
        self.EditLrc.setText(QCoreApplication.translate("PlayMP4Page", u"\u7f16\u8f91\u6b4c\u8bcd", None))
        self.FullScreenInNMP3.setText(QCoreApplication.translate("PlayMP4Page", u"\u8f6f\u4ef6\u5185\u5168\u5c4f", None))
        self.FullSreen.setText(QCoreApplication.translate("PlayMP4Page", u"\u5168\u5c4f\u663e\u793a", None))
        self.ClearNowPlayingSongList.setText(QCoreApplication.translate("PlayMP4Page", u"\u6e05\u7a7a\u5f53\u524d\u64ad\u653e\u961f\u5217", None))
        self.label.setText(QCoreApplication.translate("PlayMP4Page", u"\u97f3\u91cf\u4fee\u6539\u5230\u539f\u6765\u7684", None))
        self.label_3.setText(QCoreApplication.translate("PlayMP4Page", u"%", None))
        self.ConfirmAdjustSongVolumeByPercentage.setText(QCoreApplication.translate("PlayMP4Page", u"\u786e\u8ba4", None))
        self.label_2.setText(QCoreApplication.translate("PlayMP4Page", u"\u97f3\u91cf\u589e\u51cf\u5206\u8d1d", None))
        self.label_4.setText(QCoreApplication.translate("PlayMP4Page", u"dB", None))
        self.ConfirmAdjustSongVolumeBy_dB.setText(QCoreApplication.translate("PlayMP4Page", u"\u786e\u8ba4", None))
    # retranslateUi

