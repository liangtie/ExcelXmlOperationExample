#include "XmlModel.h"
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QStack>
#include <QDebug>
#include <QTreeView>

Q_DECLARE_METATYPE(QDomCharacterData);
static const QString &nodeTypeName(QDomNode::NodeType nodeType) {
    static QString Element("Element"), Attribute("Attribute"), Text("Text"),
            CDATA("CData"), EntityReference("Entity Reference"), Entity("Entity"),
            ProcessingInstruction("Processing Instruction"), Comment("Comment"),
            Document("Document"), DocumentType("Document Type"),
            DocumentFragment("DocumentFragment"), Notation("Notation"),
            Base("Untyped?"), CharacterData("CharacterData");
    switch(nodeType) {
    case QDomNode::ElementNode:               return Element;
    case QDomNode::AttributeNode:             return Attribute;
    case QDomNode::TextNode:                  return Text;
    case QDomNode::CDATASectionNode:          return CDATA;
    case QDomNode::EntityReferenceNode:       return EntityReference;
    case QDomNode::EntityNode:                return Entity;
    case QDomNode::ProcessingInstructionNode: return ProcessingInstruction;
    case QDomNode::CommentNode:               return Comment;
    case QDomNode::DocumentNode:              return Document;
    case QDomNode::DocumentTypeNode:          return DocumentType;
    case QDomNode::DocumentFragmentNode:      return DocumentFragment;
    case QDomNode::NotationNode:              return Notation;
    case QDomNode::BaseNode:                  return Base;
    case QDomNode::CharacterDataNode:         return CharacterData;
    }
}

XmlModel::XmlModel(QObject *parent) : QStandardItemModel(parent),
    m_lastElementItem(nullptr)
{

}

void XmlModel::loadXml(const QString &fileName)
{
    /**
     * @brief clear
     */
    clear();
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(nullptr, tr("XML editor"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return;
    }
    /**
     * @brief load device
     */
    auto device = &file;
    {
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!m_xml.setContent(device, true, &errorStr, &errorLine,
                              &errorColumn)) {
            QMessageBox::information(nullptr, tr("XML Resolve"),
                                     tr("Parse error at line %1, column %2:\n%3")
                                     .arg(errorLine)
                                     .arg(errorColumn)
                                     .arg(errorStr));
            return;
        }


    }
    /**
     *  @brief resolve infor
     */
    m_elementMap.clear();
    m_lastElementItem = nullptr;
    {
        m_rootItem = m_xml.documentElement();
        QStack<QDomNode> stack;
        QDomNode current = m_rootItem;

        while(1) {
            if (!current.isNull()) {
                addNode(current, stack.count());
                stack.push(current);
                current = current.firstChild();
            } else if (!stack.count()) {
                break;
            } else {
                current = stack.pop().nextSibling();
            }
        }

    }
    update();


}


bool XmlModel::saveXmlAs(const QString &saveName)
{
    qDebug() << __LINE__ <<saveName;
    QFile file(saveName);
    if(! file.open(QIODevice::ReadWrite)){
        qDebug() << "Open file to save failed";
        return false;
    }
    QTextStream out(&file);
    m_xml.save(out, IndentSize);
    file.close();
    return true;

}

void XmlModel::syncCaculateResult(const QVector<QPair<QString, double> > &resultList)
{
    for(auto i : resultList){
        if(m_elementMap.contains(i.first)){
            auto targetTreeItem =  m_elementMap.value(i.first).first;
            auto reseverdString =  targetTreeItem->data(Role_NumberReserved).toString();
            auto lastCalcValue = targetTreeItem->data(Role_NumberForCalc).value<double>();
            //这里改正负
            double result = lastCalcValue + i.second ;
            auto targetString = reseverdString + QString("%1").arg(QString::number(result ,'g' ,DoubelPrecious));
            auto characterDat =  targetTreeItem->data(Role_NodeString).value<QDomCharacterData>();
            characterDat.setData(targetString);
            targetTreeItem->appendRow(new QStandardItem(targetString +"(新)"));
        }
    }
    update();

}

void XmlModel::update()
{
    if(parent() && parent()->inherits("QTreeView")){
        qobject_cast<QTreeView*>(parent())->repaint();
        qobject_cast<QTreeView*>(parent())->expandAll();
    }

}
void XmlModel::addNode(QDomNode node, int depth)
{
    switch(node.nodeType()) {
    case QDomNode::ElementNode: {
        auto modelItem = new QStandardItem;
        QDomElement element = node.toElement();
        modelItem->setText(element.tagName());
        QDomNamedNodeMap attributes = element.attributes();
        for(int c = 0; c < attributes.length(); c++) {
            QDomNode attributeNode = attributes.item(c);
            QDomAttr attribute = attributeNode.toAttr();
            if (!attribute.isNull()) {
                auto attriItem = new QStandardItem;
                attriItem->setText(attribute.value());
                attriItem->setData(attribute.name());
                modelItem->appendRow(attriItem);
            }
        }
        m_elementMap.insert(element.tagName() , QPair<QStandardItem* , QDomElement>{modelItem ,element});
        m_lastElementItem = modelItem;
        appendRow(modelItem);
    }
        break;
        // These should be impossible
    case QDomNode::AttributeNode:
    case QDomNode::BaseNode:
    case QDomNode::CharacterDataNode:
        // I don't know what these are
    case QDomNode::ProcessingInstructionNode:
    case QDomNode::DocumentNode:
    case QDomNode::DocumentTypeNode:
    case QDomNode::DocumentFragmentNode:
    case QDomNode::NotationNode:
    case QDomNode::CDATASectionNode:
    case QDomNode::EntityReferenceNode:
    case QDomNode::EntityNode:
        break;
    case QDomNode::TextNode:
    case QDomNode::CommentNode: {
        auto resolveStr = [&]( QString str){
            QPair<QString ,double> res{"",0.0};
            if(3 != str.split(' ').size())
                return res;
            res.second = str.split(' ').last().toDouble();
            res.first = str.replace(str.split(' ').last(),"");
            return res;
        };
        QDomCharacterData data = node.toCharacterData();     
        if(m_lastElementItem){
            auto resolvedInfor = resolveStr(data.data().trimmed());
            m_lastElementItem->appendRow( new QStandardItem(data.data()));
            m_lastElementItem->setData(QVariant::fromValue(data) , Role_NodeString);
            m_lastElementItem->setData(resolvedInfor.first ,Role_NumberReserved);
            m_lastElementItem->setData(resolvedInfor.second ,Role_NumberForCalc);
        }
    } break;
    }



}
