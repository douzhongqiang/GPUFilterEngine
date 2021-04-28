#ifndef GPUFILTERNODEBASE_H
#define GPUFILTERNODEBASE_H

#include <QVector>

// Search Node
template <typename T>
class EeoSearchNode
{
public:
    EeoSearchNode(){}
    EeoSearchNode(const T& data) : m_data(data){}
    ~EeoSearchNode(){
        for (auto iter = m_searchNodes.begin(); iter != m_searchNodes.end(); ++iter)
            delete (*iter);
        m_searchNodes.clear();
    }

    void addNode(EeoSearchNode* pNode){
        pNode->parentNode = this;
        m_searchNodes.append(pNode);
    }

    void setData(const T& data){
        m_data = data;
    }

    T getData(void){
        return m_data;
    }

    T& getDataReference(void) {
        return m_data;
    }

    EeoSearchNode* getParentNode(void){
        return parentNode;
    }

    QList<EeoSearchNode*> getChilds(void){
        return m_searchNodes;
    }

    int getChildCount(void) {
        return m_searchNodes.size();
    }

    EeoSearchNode* findChild(const T& data){
        for (auto iter = m_searchNodes.begin(); iter != m_searchNodes.end(); ++iter){
            if ((*iter)->getData() == data)
                return *iter;
        }

        return nullptr;
    }

    bool isLastNode(void) {
        if (parentNode == nullptr)
            return false;

        QList<EeoSearchNode*> childNodes = parentNode->getChilds();
        if (childNodes.size() <= 0)
            return false;

        if (childNodes.at(childNodes.size() - 1) == this)
            return true;

        return false;
    }

private:
    EeoSearchNode* parentNode = nullptr;
    QList<EeoSearchNode*> m_searchNodes;

    T m_data;
};

#endif
