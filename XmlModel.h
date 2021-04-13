#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QDomDocument>

#include <QPlainTextEdit>
#include <QDomCharacterData>
#include <QLineEdit>
#include <QDomElement>

class CharacterDataWatcher : public QObject {
    Q_OBJECT
protected:
    QPlainTextEdit* edit;
    QDomCharacterData data;
public:
    CharacterDataWatcher(QPlainTextEdit* _edit, const QDomCharacterData &_data) : QObject(_edit), edit(_edit), data(_data) {
        connect(edit, &QPlainTextEdit::textChanged,
            this, &CharacterDataWatcher::changed);
    }
public Q_SLOTS:
    void changed() {
        data.setData(edit->toPlainText());
    }
};

class TagNameWatcher : public QObject {
    Q_OBJECT
protected:
    QLineEdit* edit;
    QDomElement data;
public:
    TagNameWatcher(QLineEdit* _edit, const QDomElement &_data) : QObject(_edit), edit(_edit), data(_data) {
        connect(edit, &QLineEdit::textChanged,
            this, &TagNameWatcher::changed);
    }
public Q_SLOTS:
    void changed() {
        data.setTagName(edit->text());
    }
};

class AttrWatcher : public QObject {
    Q_OBJECT
protected:
    QString previousName;
    QLineEdit* nameEdit;
    QLineEdit* valueEdit;
    QDomElement data;
public:
    AttrWatcher(QLineEdit* _nameEdit, QLineEdit* _valueEdit, const QDomElement &_data) : QObject(_nameEdit), nameEdit(_nameEdit), valueEdit(_valueEdit), data(_data) {
        previousName = nameEdit->text();
        connect(nameEdit, &QLineEdit::textChanged,
            this, &AttrWatcher::nameChanged);
        connect(valueEdit, &QLineEdit::textChanged,
            this, &AttrWatcher::valueChanged);
    }
public Q_SLOTS:
    void nameChanged() {
        data.removeAttribute(previousName);
        previousName = nameEdit->text();
        data.setAttribute(previousName, valueEdit->text());
    }
    void valueChanged() {
        data.setAttribute(previousName, valueEdit->text());
    }
};


class XmlModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum {
        IndentSize = 4,
        Role_NodeString = Qt::UserRole + 11,
        Role_NumberReserved,
        Role_NumberForCalc,

    };
    enum Columns{
        NodeName,
        ConfigBefore,
        ConfigNew,
        ColumCount,

    };
    enum{
        DoubelPrecious = 17,
    };

    explicit XmlModel(QObject *parent = nullptr);
    void loadXml(const QString &filePath);
    bool saveXmlAs(const QString&saveName);
    void syncCaculateResult(const QVector<QPair<QString ,double>>& resultList);

private:
    QDomDocument m_xml;
    void setNodeAttibution();
    void update();
    void addNode(QDomNode node, int depth);
    QStandardItem* m_lastElementItem ;
    QDomElement m_rootItem;
    QMap<QString , QPair<QStandardItem*, QDomElement>> m_elementMap;

signals:

};

#endif // XMLMODEL_H
