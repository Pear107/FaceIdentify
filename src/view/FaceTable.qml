import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.13
import Qt.labs.qmlmodels 1.0
import QtGraphicalEffects 1.0

Window {
    width: 600
    height: 400
    maximumWidth: 600
    visible: false
    title: '表格'
    color: "#9E9E9E"
    Row {
        id: columnHeader
        width: parent.width
        Rectangle{
            width: 60
            height: 32
            border.width: 1
            border.color: "#80CBC4"
            color: "#00796B"
            Text {
                text: "项"
                anchors.centerIn: parent
                font.pointSize: 12
                color: "white"
            }
        }
        Rectangle{
            width: 140
            height: 32
            border.width: 1
            border.color: "#80CBC4"
            color: "#00796B"
            Text {
                text: "姓名"
                anchors.centerIn: parent
                font.pointSize: 12
                color: "white"
            }
        }
        Rectangle{
            width: 200
            height: 32
            border.width: 1
            border.color: "#80CBC4"
            color: "#00796B"
            Text {
                text: "时间"
                anchors.centerIn: parent
                font.pointSize: 12
                color: "white"
            }
        }
        Rectangle{
            width: 200
            height: 32
            border.width: 1
            border.color: "#80CBC4"
            color: "#00796B"
            Text {
                text: "操作"
                anchors.centerIn: parent
                font.pointSize: 12
                color: "white"
            }
        }
    }
    TableView {
        id: tableView
        anchors.top: columnHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true
        // columnSpacing: 1
        // rowSpacing: 1
        // 关闭拖动
        boundsBehavior: Flickable.OvershootBounds
        columnWidthProvider: function (column) {
            switch(column){
            case 0:
                return 60
            case 1:
                return 140
            default:
                return 200
            }
        }
        rowHeightProvider: function (column) { return 32; }
        model: TableModel {
            id:tableModel
            TableModelColumn{display: "id"}
            TableModelColumn{display: "name"}
            TableModelColumn{display: "time"}
            TableModelColumn{display: "option"}
        }
        delegate:DelegateChooser{
            DelegateChoice{
                column: 0
                Rectangle{
                    width: 60
                    height: 32
                    border.width: 1
                    border.color: "#80CBC4"
                    color: "#009688"
                    Text {
                        text: index
                        anchors.centerIn: parent
                        font.pointSize: 12
                        color: "white"
                    }
                }
            }
            DelegateChoice{
                column: 1
                delegate: Rectangle{
                    border.width: 1
                    border.color: "#80CBC4"
                    color: "#E0F2F1"
                    Text {
                        text: display
                        anchors.centerIn: parent
                        font.pointSize: 12
                    }
                }
            }
            DelegateChoice{
                column: 2
                delegate: Rectangle{
                    border.width: 1
                    border.color: "#80CBC4"
                    color: "#E0F2F1"
                    Text {
                        text: display
                        anchors.centerIn: parent
                        font.pointSize: 12
                    }
                }
            }
            DelegateChoice{
                column: 3
                delegate: Rectangle {
                    border.width: 1
                    border.color: "#80CBC4"
                    color: "#E0F2F1"
                    Row {
                        spacing: 10
                        anchors.centerIn: parent
                        Rectangle {
                            width: 70
                            height: 30
                            color: "#E0F2F1"
                            Button{
                                anchors.fill: parent
                                width: parent.width
                                height: parent.height
                                id: updateBtn
                                display: AbstractButton.TextBesideIcon
                                background: Rectangle {
                                    radius: 4
                                    color: "#4CAF50"
                                }
                                contentItem: Label{
                                    text: "更新"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: "#fff"
                                }
                            }
                            DropShadow {
                                anchors.fill: updateBtn
                                radius: 8.0
                                samples: 16
                                color: "#607D8B"
                                source: updateBtn
                            }
                        }
                        Rectangle {
                            width: 70
                            height: 30
                            color: "#E0F2F1"
                            Button{
                                anchors.fill: parent
                                width: parent.width
                                height: parent.height
                                id: removeBtn
                                display: AbstractButton.TextBesideIcon
                                background: Rectangle {
                                    radius: 4
                                    color: "#F44336"
                                }
                                contentItem: Label{
                                    text: "删除"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    color: "#fff"
                                }
                            }
                            DropShadow {
                                anchors.fill: removeBtn
                                radius: 8.0
                                samples: 16
                                color: "#607D8B"
                                source: removeBtn
                            }
                        }
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        tableModel.appendRow({"id":"0", "name":"小明", "time":"test", "option": true})
        tableModel.appendRow({"id":"0", "name":"小刚", "time":"test", "option": true})
        tableModel.appendRow({"id":"0", "name":"小李", "time":"test", "option": true})
        tableModel.appendRow({"id":"0", "name":"小王", "time":"test", "option": true})
        tableModel.appendRow({"id":"0", "name":"小张", "time":"test", "option": true})
        tableModel.appendRow({"id":"0", "name":"小林", "time":"test", "option": true})
    }
}
