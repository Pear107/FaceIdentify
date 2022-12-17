import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import Qt.labs.platform 1.1 as Labs
import Custom.Screen 1.0

ApplicationWindow {
    x: 460
    y: 200
    width: 1000
    height: 680
    visible: true
    title: qsTr("人脸检测系统")
    property int _classifier: 0
    header: ToolBar {
        RowLayout {
            ToolButton {
                id: photo
                text: qsTr("图片")
                onClicked: {
                    fileDialog.open()
                }
                Labs.FileDialog {
                    title: qsTr("打开图片或者txt文件")
                    id: fileDialog
                    nameFilters: ["image files (*.png *.jpg *.jpeg *.bmp)"]
                    acceptLabel: qsTr("确定")
                    rejectLabel: qsTr("取消")
                    fileMode: Labs.FileDialog.OpenFile
                    onAccepted: {
                        const filePath = fileDialog.files[0]
                        switch(screen.model){
                        case 0:
                            screen.detectPhoto(filePath.replace("file:///",""))
                            break;
                        case 1:
                            screen.recoPhoto(filePath.replace("file:///",""))
                            break
                        case 2:
                            screen.trainPhoto(filePath.replace("file:///",""))
                            break
                        default:
                            break
                        }
                    }
                }
            }
            ToolButton {
                id: vidicon
                text: qsTr("打开摄像机")
                onClicked: {
                    screen.isOpen = !screen.isOpen;
                    if(screen.isOpen){
                        photo.enabled = false;
                        model.enabled = false;
                        save.enabled = false;
                        exit.enabled = false;
                        vidicon.text = "关闭摄像机";
                        console.log("摄像机已打开");
                        foot.text="训练中"
                    }else{
                        photo.enabled = true;
                        model.enabled = true;
                        save.enabled = true;
                        exit.enabled = true;
                        vidicon.text = "打开摄像机";
                        console.log("摄像机已关闭");
                        foot.text="暂停训练"
                    }
                }
                Connections{
                    target: screen
                    function onSendFinish(){
                        image.enabled = true;
                        model.enabled = true;
                        save.enabled = true;
                        exit.enabled = true;
                        vidicon.text = "打开摄像机";
                        foot.text = "训练完成"
                    }
                }
            }
            Label {
                text: qsTr("模式")
            }
            ComboBox {
                id: model
                model:ListModel {
                    ListElement {
                        text: qsTr("检测模式")
                    }
                    ListElement {
                        text: qsTr("识别模式")
                    }
                    ListElement {
                        text: qsTr("训练模式")
                    }
                }
                //                background: Rectangle {
                //                    implicitWidth: 100
                //                    implicitHeight: 20
                //                    border.width: 1
                //                    radius: 2
                //                }
                onActivated: {
                    screen.model = currentIndex
                }
                //                delegate: ItemDelegate {
                //                     width: model.width
                //                     contentItem: Text {
                //                         text: modelData
                //                         color: "green"
                //                         font: model.font
                //                         verticalAlignment: Text.AlignVCenter
                //                     }
                //                 }
            }
            ToolButton {
                id: save
                text: qsTr("保存")
                enabled: false
            }
            ToolButton {
                id: data
                text: qsTr("数据")
                onClicked: {
                    faceTable.show()
                }
            }
            Label {
                text: qsTr("分类器")
            }
            ComboBox {
                id: classifer
                model: ListModel{
                    ListElement {
                        text: qsTr("脸")
                    }
                    ListElement {
                        text: qsTr("眼睛")
                    }
                    ListElement {
                        text: qsTr("嘴巴")
                    }
                    ListElement {
                        text: qsTr("鼻子")
                    }
                    ListElement {
                        text: qsTr("左耳")
                    }
                    ListElement {
                        text: qsTr("右耳")
                    }
                }
//                editable: true
                onActivated: {
                    if(_classifier === currentIndex){
                        return
                    }else{
                        _classifier = currentIndex
                    }
                    switch(currentIndex){
                    case 0:
                        cover.model = ["无", "面具"]
                        break
                    case 1:
                        cover.model = ["无", "眼镜"]
                        break
                    case 3:
                        cover.model = ["无", "红鼻子"]
                        break
                    default:
                        cover.model = ["无"]
                    }
                    screen.setCover(0)
                    screen.setClassifier(currentIndex);
                }
            }
            Label {
                text: qsTr("遮盖物")
            }
            ComboBox {
                id: cover
                model:["无", "面具"]
                onActivated: {
                    screen.setCover(currentIndex);
                }
            }
            ToolButton {
                text: qsTr("帮助")
            }
            ToolButton {
                text: qsTr("关于")
                onClicked: {

                }
            }
            ToolButton {
                id: exit
                text: qsTr("退出")
                onClicked: {
                    Qt.quit()
                }
            }
        }
    }
    CScreen {
        id: screen
        anchors.fill: parent
        isOpen: false
        model: 0
    }
    Rectangle {
        width: parent.width
        height: 20
        anchors.bottom: screen.bottom;
        Text {
            height: 20
            id: foot
            text: qsTr("")
        }
    }
    FaceTable {
        id: faceTable
    }
}
