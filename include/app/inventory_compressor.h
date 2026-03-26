#ifndef INVENTORY_COMPRESSOR_H
#define INVENTORY_COMPRESSOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include "inventory.h"
#include "Hash/xMap.h"
#include "Heap/Heap.h"
#include "list/XArrayList.h"

namespace std {
    inline ostream& operator<<(ostream& os, const pair<char, int>& p) {
        os << "(" << p.first << ", " << p.second << ")";
        return os;
    }
}
template<int treeOrder>
class HuffmanTree {
public:
    struct HuffmanNode {
        char symbol;
        int freq;
        XArrayList<HuffmanNode*> children;

        int id;
        static int ID;

        HuffmanNode(char s, int f): symbol(s), freq(f), children() 
        {
            this->id = ID++;
        }
        HuffmanNode(int f, const XArrayList<HuffmanNode*>& childs)
                    : symbol('\0'), freq(f), children(childs) 
        {
            this->id = ID++;
        }
    };
    static int minComparator(HuffmanTree<treeOrder>::HuffmanNode*& lhs, HuffmanTree<treeOrder>::HuffmanNode*& rhs) 
    {
        if (lhs->freq < rhs->freq) return -1;
        if (lhs->freq > rhs->freq) return 1;
    
        if (lhs->id < rhs->id) return -1;
        if (lhs->id > rhs->id) return 1;
        return 0;
    }
    static int charHash(char& key, int tablesize) {
        return (int)key % tablesize;
    } 

    HuffmanTree();
    ~HuffmanTree();

    void build(XArrayList<pair<char, int>>& symbolsFreqs);
    void generateCodes(xMap<char, std::string>& table);
    std::string decode(const std::string& huffmanCode);

    void helpGenerate(HuffmanNode* node, const std::string& path, xMap<char, std::string> &table);
    void clear(HuffmanNode* node);
private:
    HuffmanNode* root;
};

template<int treeOrder>
class InventoryCompressor {
public:
    static int charHash(char& key, int tablesize) {
        return (int)key % tablesize;
    }

    InventoryCompressor(InventoryManager* manager);
    ~InventoryCompressor();

    void buildHuffman();
    void printHuffmanTable();
    std::string productToString(const List1D<InventoryAttribute>& attributes, const std::string& name);
    std::string encodeHuffman(const List1D<InventoryAttribute>& attributes, const std::string& name);
    std::string decodeHuffman(const std::string& huffmanCode, List1D<InventoryAttribute>& attributesOutput, std::string& nameOutput);
private:
    xMap<char, std::string>* huffmanTable;
    InventoryManager* invManager;
    HuffmanTree<treeOrder>* tree;
};


#endif // INVENTORY_COMPRESSOR_H
template <int treeOrder>
int HuffmanTree<treeOrder>::HuffmanNode::ID = 0;

template <int treeOrder>
HuffmanTree<treeOrder>::HuffmanTree()
{
    this->root = nullptr;
}

template <int treeOrder>
HuffmanTree<treeOrder>::~HuffmanTree()
{
    //TODO
    clear(root);
}

template <int treeOrder>
void HuffmanTree<treeOrder>::clear(HuffmanNode* node)
{
    //TODO
    if (node == nullptr) return;
    for (int i = 0; i < node->children.size(); ++i) {
        clear(node->children.get(i));
    }
    delete node;
}

template <int treeOrder>
void HuffmanTree<treeOrder>::build(XArrayList<pair<char, int>>& symbolsFreqs)
{
    //TODO
    Heap<HuffmanNode*> heap(minComparator, nullptr);
    for (int i = 0; i < symbolsFreqs.size(); i++)
    {
        HuffmanNode* node = new HuffmanNode(symbolsFreqs.get(i).first, symbolsFreqs.get(i).second);
        heap.push(node);
    }

    if (symbolsFreqs.size() == 1)
    {
        HuffmanNode* node = new HuffmanNode('\0', 0);
        heap.push(node);
    }
    while ( (heap.size() - 1) % (treeOrder - 1)  > 0)
    {
        HuffmanNode* node = new HuffmanNode('\0', 0);
        heap.push(node);
    }

    int sum;
    while (heap.size() > 1) {
        sum = 0;
        XArrayList<HuffmanNode*> childs;
        for (int i = 0; i < treeOrder && !heap.empty(); i++)
        {
            HuffmanNode* node = heap.peek();
            sum += node->freq;
            childs.add(node); 
            heap.pop();
        }
        HuffmanNode* parent = new HuffmanNode(sum, childs);
        heap.push(parent);
    }
    root = heap.peek();
}

template <int treeOrder>
void HuffmanTree<treeOrder>::generateCodes(xMap<char, std::string> &table)
{
    //TODO
    helpGenerate(root, "", table);
}
template <int treeOrder>
void HuffmanTree<treeOrder>::helpGenerate(HuffmanNode* node, const std::string& path, xMap<char, std::string> &table) {
    if (!node) return;

    if (node->children.size() == 0) 
    {
        if (node->symbol != '\0')
        {
            table.put(node->symbol, path);
        }
        return;
    }

    for (int i = 0; i < node->children.size(); ++i) 
    {
        stringstream temp;
        if ( i < 10)
        {
            temp << i;
        }
        else
        {
            char ch = 'a' + (i - 10);
            temp << ch;
        }
        helpGenerate(node->children.get(i), path + temp.str(), table);
    }
}

template <int treeOrder>
std::string HuffmanTree<treeOrder>::decode(const std::string &huffmanCode)
{
    //TODO
    xMap<char, std::string> table(charHash);
    generateCodes(table);

    stringstream result;
    stringstream temp;
    for (int i = 0; i < huffmanCode.size(); ++i)
    {
        temp << huffmanCode[i];           
        if ( table.containsValue( temp.str() ) )
        {
            result << table.getKey( temp.str() );
            temp.str("");
        }
    }
 
    return result.str();
}

template <int treeOrder>
InventoryCompressor<treeOrder>::InventoryCompressor(InventoryManager *manager)
                              : huffmanTable(new xMap<char, std::string>(charHash)), 
                                invManager(manager), 
                                tree( new HuffmanTree <treeOrder>() ) {}

template <int treeOrder>
InventoryCompressor<treeOrder>::~InventoryCompressor()
{
    //TODO
    if (huffmanTable != nullptr)
    {
        delete huffmanTable;
    }
    if (tree != nullptr)
    {
        delete tree;
    }
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::buildHuffman()
{
    //TODO
    XArrayList<pair<char, int>> res;
    stringstream s;
    for (int i = 0; i < invManager->size(); i++)
    {
        string temp = this->productToString(invManager->getProductAttributes(i), 
                                            invManager->getProductName(i));
        s << temp;
    }

    string temp = s.str();
    xMap<char, int> hash(charHash);
    for (char c : temp) 
    {
        if ( hash.containsKey(c))
        {
            hash.get(c)++;
        }
        else hash.put(c, 1);
    }

    DLinkedList<char> keys = hash.keys(); 
    DLinkedList<int> values = hash.values();
    for (int i = 0; i < keys.size(); i++) 
    {
        res.add( make_pair(keys.get(i), values.get(i)) );
    }

    tree->build(res);
    tree->generateCodes(*huffmanTable);
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::printHuffmanTable() {
    DLinkedList<char> keys = huffmanTable->keys();
    for (char ch : keys) {
        std::cout << "'" << ch << "' : " << huffmanTable->get(ch) << std::endl;
    }
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::productToString(const List1D<InventoryAttribute> &attributes, const std::string &name)
{
    //TODO
    stringstream result;
    result << name << ":";
    for (int i = 0; i < attributes.size(); i++)
    {
        result << "(" << attributes.get(i).name << ":" << to_string(attributes.get(i).value) << ")";
        if (i < attributes.size() - 1)
        {
            result << ", ";
        }
    }

    return result.str();
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::encodeHuffman(const List1D<InventoryAttribute> &attributes, const std::string &name)
{
    //TODO
    string str = productToString(attributes, name);
    stringstream s;
    for (int i = 0; i < str.size(); i++)
    {
        s << huffmanTable->get(str[i]);
    }
    return s.str();
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::decodeHuffman(const std::string &huffmanCode, List1D<InventoryAttribute> &attributesOutput, std::string &nameOutput)
{
    //TODO
    string str = tree->decode(huffmanCode);

    nameOutput = str.substr(0, str.find(':', 0)); 

    size_t start = 0;
    while ( (start = str.find('(', start)) != std::string::npos) 
    {
        size_t end = str.find(')', start);
        if (end != std::string::npos) 
        {
            std::string attr = str.substr(start + 1, end - start - 1);

            size_t tmp = attr.find(':');
            if (tmp != std::string::npos) 
            {
                std::string key = attr.substr(0, tmp);
                std::string value = attr.substr(tmp + 1);

                InventoryAttribute temp(key, stod(value));
                attributesOutput.add(temp);
            }

            start = end + 1;
        } 
        else 
        {
            break;
        }
    }
    return str;

}


