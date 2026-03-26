#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include "list/XArrayList.h"
#include "list/DLinkedList.h"
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;




// -------------------- List1D --------------------
template <typename T>
class List1D
{
private:
    IList<T> *pList;

public:
    List1D();
    List1D(int num_elements);
    List1D(const T *array, int num_elements);
    List1D(const List1D<T> &other);
    virtual ~List1D();
    List1D<T> &operator=(const List1D<T> &other); 
    int size() const;
    T get(int index) const;
    void set(int index, T value);
    void add(const T &value);

    //! thêm hàm này 
    string toString() const;
    template <typename U> //! thêm vào  để chạy test 
    friend ostream &operator<<(ostream &os, const List1D<T> &list);

    void removeAt(int index);
    void add_index(int index, T value);
};

template <typename T>
class List2D
{
private:
    IList<IList<T> *> *pMatrix;

public:
    List2D();
    List2D(List1D<T> *array, int num_rows);
    List2D(const List2D<T> &other);
    virtual ~List2D();
    List2D<T> &operator=(const List2D<T> &other);
    int rows() const;
    //! thêm hàm này 
    
    void setRow(int rowIndex, const List1D<T> &row);
    T get(int rowIndex, int colIndex) const;
    List1D<T> getRow(int rowIndex) const;
    string toString() const;
    template <typename U> //! thêm vào  để chạy test 
    friend ostream &operator<<(ostream &os, const List2D<T> &matrix);

    void removeAt(int rowIndex);
};
struct InventoryAttribute
{
    string name;
    double value;
    //! thêm
    InventoryAttribute() : name(""), value(0.0) {}  // Constructor mặc định
    InventoryAttribute(const string &name, double value) : name(name), value(value) {}
    string toString() const { return name + ": " + to_string(value); }
     //! thêm
     // Định nghĩa toán tử so sánh ==
     bool operator==(const InventoryAttribute& other) const {
        return name == other.name && value == other.value;
    }
      // Toán tử in ra ostream
      friend std::ostream &operator<<(std::ostream &os, const InventoryAttribute &attr) {
        return os << attr.toString();
    }
};

// -------------------- InventoryManager --------------------
class InventoryManager
{
private:
    List2D<InventoryAttribute> attributesMatrix;
    List1D<string> productNames;
    List1D<int> quantities;

public:
    InventoryManager();
       
    
    InventoryManager(const List2D<InventoryAttribute> &matrix,
                     const List1D<string> &names,
                     const List1D<int> &quantities);
                     
                     
    InventoryManager(const InventoryManager &other);

    int size() const;
       
    
    List1D<InventoryAttribute> getProductAttributes(int index) const;
    string getProductName(int index) const;
    int getProductQuantity(int index) const;
    void updateQuantity(int index, int newQuantity);
    void addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity);
    void removeProduct(int index);
    List1D<string> query(string attributeName, const double &minValue,
                         const double &maxValue, int minQuantity, bool ascending) const;
    void removeDuplicates();

    static InventoryManager merge(const InventoryManager &inv1,
                                  const InventoryManager &inv2);

    void split(InventoryManager &section1,
               InventoryManager &section2,
               double ratio) const;        

    List2D<InventoryAttribute> getAttributesMatrix() const;
    List1D<string> getProductNames() const;
    List1D<int> getQuantities() const;
    string toString() const;

    int quantityFromName(string name) const;
};

// -------------------- List1D Method Definitions --------------------
template <typename T>
List1D<T>::List1D()
{
    // TODO
    this->pList = new XArrayList<T>();
}

template <typename T>
List1D<T>::List1D(int num_elements)
{
    // TODO
    this->pList = new XArrayList<T>();
    for (int i = 0; i < num_elements; i++)
    {
        this->pList->add(T());
    }
    
}

template <typename T>
List1D<T>::List1D(const T *array, int num_elements)
{
    // TODO
    this->pList = new XArrayList<T>();
    for (int i = 0; i < num_elements; i++)
    {
        this->pList->add(array[i]);
    }
}

template <typename T>
List1D<T>::List1D(const List1D<T> &other)
{
    // TODO
    if (this != &other)
    {
        this->pList = new XArrayList<T>();
        for (int i = 0; i < other.size(); i++)
        {
            this->add(other.get(i));
        }
    }

}

template <typename T>
List1D<T>::~List1D()
{
    // TODO
    delete pList; 
}
template <typename T>
List1D<T> &List1D<T>::operator=(const List1D<T> &other)
{
    if (this != &other)
    {
        if (this->pList != nullptr && this->size() != 0)
        {
            delete this->pList;
            this->pList = nullptr;
        }
        
        if (this->pList == nullptr)
        {
            this->pList = new XArrayList<T>();
        }
        for (int i = 0; i < other.size(); i++)
        {
            this->add(other.get(i));
        }
    }
    return *this;
}

template <typename T>
int List1D<T>::size() const
{
    // TODO
    return this->pList->size(); 
}

template <typename T>
T List1D<T>::get(int index) const
{
    // TODO
    return this->pList->get(index);
   
}

template <typename T>
void List1D<T>::set(int index, T value)
{
    // TODO
    if ( index >= 0 && index <= this->size())
    {
        if (index == this->size())
        {
            this->pList->add(value);
        }
        else
        {
            this->pList->removeAt(index);
            this->pList->add(index, value);
        }
    }
    else throw std::out_of_range("Index is out of range!");  
}

template <typename T>
void List1D<T>::add(const T &value)
{
    // TODO
    this->pList->add(value);
}

template <typename T>
string List1D<T>::toString() const
{
    // TODO
    return this->pList->toString();
}

template <typename T>
ostream &operator<<(ostream &os, const List1D<T> &list)
{
    os << list.toString();
    return os;
}

template <typename T>
void List1D<T>::removeAt(int index)
{
   this->pList->removeAt(index);
}

template <typename T>
void List1D<T>::add_index(int index, T value)
{
    this->pList->add(index, value);
}

// -------------------- List2D Method Definitions --------------------
template <typename T>
List2D<T>::List2D()
{
    // TODO
    pMatrix = new XArrayList<IList<T> *>();
}
template <typename T>
List2D<T> &List2D<T>::operator=(const List2D<T> &other)
{
    if (this != &other)
    {
        if (this->pMatrix != nullptr && this->rows() != 0)
        {
            int size = this->rows();
            for (int i = 0; i < size; i++)
            {
                delete pMatrix->removeAt(0);
            }
            delete pMatrix;  
            pMatrix = nullptr; 
        }
        
        if (this->pMatrix == nullptr)
        {
            pMatrix = new XArrayList<IList<T> *>();
        }

        for (int i = 0; i < other.rows(); i++)
        {
            this->setRow(i, other.getRow(i));
        }
    }
    return *this;    
}

template <typename T>
List2D<T>::List2D(List1D<T> *array, int num_rows)
{
    pMatrix = new XArrayList<IList<T> *>();
    for (int i = 0; i < num_rows; i++)
    {
        this->setRow(i, array[i]);
    }   
}


template <typename T>
List2D<T>::List2D(const List2D<T> &other)
{
    // TODO
    if (this != &other)
    {
        pMatrix = new XArrayList<IList<T> *>();
        for (int i = 0; i < other.rows(); i++)
        {
            this->setRow(i, other.getRow(i));
        }
    }    
}

template <typename T>
List2D<T>::~List2D()
{
    // TODO
    int size = this->rows();
    for (int i = 0; i < size; i++)
    {
       // XArrayList<T>* temp = pMatrix->removeAt(i);
        delete pMatrix->removeAt(0);
    }
    delete pMatrix;  
    pMatrix = nullptr;   
}

template <typename T>
int List2D<T>::rows() const
{
    return pMatrix->size();
}

template <typename T>
void List2D<T>::setRow(int rowIndex, const List1D<T> &row)
{
    // TODO
    if (rowIndex >= 0 && rowIndex < this->rows())
    {
      //  XArrayList<T>* temp = pMatrix->removeAt(rowIndex);
        delete pMatrix->removeAt(rowIndex);
    }
    else if (rowIndex > this->rows())
    {
        throw std::out_of_range("Index is out of range!");
        return;
    }

    XArrayList<T>* x = new XArrayList<T>();
    for (int i = 0; i < row.size(); i++)
    {
        x->add(row.get(i));
    }
    pMatrix->add( rowIndex, x);
}

template <typename T>
T List2D<T>::get(int rowIndex, int colIndex) const
{
    // TODO
    XArrayList<T>* r = dynamic_cast<XArrayList<T>*>(pMatrix->get(rowIndex));
    return r->get(colIndex);   
}

template <typename T>
List1D<T> List2D<T>::getRow(int rowIndex) const
{
    // TODO
    if (rowIndex >= this->rows() || rowIndex < 0)
    {
        throw std::out_of_range("Index is out of range!");
    }
    else
    {
        List1D<T>* temp = new List1D<T>();
        XArrayList<T>* x = dynamic_cast<XArrayList<T>*>(pMatrix->get(rowIndex));
        for (int i = 0; i < x->size(); i++)
        {
            temp->add(x->get(i));
        }
        List1D<T> result = *temp; 
        delete temp; 
        return result;
    }  
}

template <typename T>
string List2D<T>::toString() const
{
    // TODO
    stringstream result;
    result << "[";
 
     for (int i = 0; i < this->rows(); i++)
     {
        result << pMatrix->get(i)->toString();
        if (i < this->rows() - 1)
        {
            result << ", ";
        }
     }
 
     result << "]";
 
     return result.str();   
}

template <typename T>
ostream &operator<<(ostream &os, const List2D<T> &matrix)
{
    // TODO
    os << matrix.toString();
    return os;   
}

template <typename T>
void List2D<T>::removeAt(int rowIndex)
{
    delete pMatrix->removeAt(rowIndex);
}
#endif /* INVENTORY_MANAGER_H */
