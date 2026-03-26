
#include "app/inventory.h"
// // -------------------- InventoryManager Method Definitions --------------------
InventoryManager::InventoryManager()
{
    // TODO
    this->attributesMatrix = List2D<InventoryAttribute>();
    this->productNames = List1D<string>();
    this->quantities = List1D<int>();
}

InventoryManager::InventoryManager(const List2D<InventoryAttribute> &matrix,
                                   const List1D<string> &names,
                                   const List1D<int> &quantities)
{
    // TODO
    this->attributesMatrix = matrix;
    this->productNames = names;
    this->quantities = quantities;

}

InventoryManager::InventoryManager(const InventoryManager &other)
{
    // TODO
    if (this != &other)
    {
        this->attributesMatrix = other.getAttributesMatrix();
        this->productNames = other.getProductNames();
        this->quantities = other.getQuantities();
    }
}

int InventoryManager::size() const
{
    // TODO
    return this->productNames.size();  
}

List1D<InventoryAttribute> InventoryManager::getProductAttributes(int index) const
{
    // TODO
    if (index < 0 || index >= this->size() )
    {
        throw std::out_of_range("Index is invalid!");
    }

    return this->attributesMatrix.getRow(index);
}

string InventoryManager::getProductName(int index) const
{
    // TODO
    if (index < 0 || index >= this->size() )
    {
        throw std::out_of_range("Index is invalid!");
    }

    return this->productNames.get(index);
}

int InventoryManager::getProductQuantity(int index) const
{
    // TODO
    if (index < 0 || index >= this->size() )
    {
        throw std::out_of_range("Index is invalid!");
    }

    return this->quantities.get(index);    
}

void InventoryManager::updateQuantity(int index, int newQuantity)
{
    // TODO
    if (index < 0 || index >= this->size() )
    {
        throw std::out_of_range("Index is invalid!");
    }

    this->quantities.set(index, newQuantity);
}

void InventoryManager::addProduct(const List1D<InventoryAttribute> &attributes, const string &name, int quantity)
{
    // TODO
    this->attributesMatrix.setRow(this->size(), attributes);
    this->productNames.add(name);
    this->quantities.add(quantity);   

}

void InventoryManager::removeProduct(int index)
{
    // TODO
    if (index < 0 || index >= this->size() )
    {
        throw std::out_of_range("Index is invalid!");
    }

    this->attributesMatrix.removeAt(index);
    this->productNames.removeAt(index);
    this->quantities.removeAt(index);   
}


//! tăng dần của cái gì
List1D<string> InventoryManager::query(string attributeName, const double &minValue,
                                       const double &maxValue, int minQuantity, bool ascending) const
{
    // TODO
    List1D<string> listName = List1D<string>();
    List1D<int> listValue = List1D<int>();
    for (int i = 0; i < this->size(); i++)
    {
        List1D<InventoryAttribute> temp = this->getProductAttributes(i);
        for (int j = 0; j < temp.size(); j++)
        {
            if (temp.get(j).name == attributeName)
            {
                if ( temp.get(j).value >= minValue && temp.get(j).value <= maxValue 
                                                   && this->getProductQuantity(i) >= minQuantity)
                {
                    if (ascending)
                    {
                        listName.add( this->getProductName(i) );
                        listValue.add(temp.get(j).value);
                    }
                    else
                    {
                        listName.add_index(0, this->getProductName(i));
                        listValue.add_index(0, temp.get(j).value);
                    }
                }
            }
        }
    }

    if (listName.size() != 0 && ascending)
    {
        for (int first = 1; first < listValue.size(); ++first)
        {
            int i = first;
            while ( i >= 1 && (listValue.get(i-1) > listValue.get(i) 
                              || (listValue.get(i-1) == listValue.get(i) && this->quantityFromName(listName.get(i-1)) > this->quantityFromName(listName.get(i)))) 
                  )
            {
                //swap(list[i - 1], list[i]);
                int temp1 = listValue.get(i-1);
                listValue.set(i-1, listValue.get(i));
                listValue.set(i, temp1);

                string temp2 = listName.get(i-1);
                listName.set(i-1, listName.get(i));
                listName.set(i, temp2);
                --i;
            }
        }
    }
    else if (listName.size() != 0 && !ascending)
    {
        for (int first = 1; first < listValue.size(); ++first)
        {
            int i = first;
            while ( i >= 1 && (listValue.get(i-1) < listValue.get(i) 
                              || (listValue.get(i-1) == listValue.get(i) && this->quantityFromName(listName.get(i-1)) < this->quantityFromName(listName.get(i)))) 
                   )
            {
                //swap(list[i - 1], list[i]);
                int temp1 = listValue.get(i-1);
                listValue.set(i-1, listValue.get(i));
                listValue.set(i, temp1);

                string temp2 = listName.get(i-1);
                listName.set(i-1, listName.get(i));
                listName.set(i, temp2);

                --i;
            }
        }
    }
    return listName;   
}


void InventoryManager::removeDuplicates()
{
    // TODO
    for (int i = 0; i < this->size(); i++)
    {
        int j = i + 1;
        while (j < this->size())
        {
            if (this->getProductAttributes(i).toString() == this->getProductAttributes(j).toString()
                 && this->getProductName(i) == this->getProductName(j) )
            {
                this->updateQuantity(i, this->getProductQuantity(i) + this->getProductQuantity(j));
                this->attributesMatrix.removeAt(j);
                this->productNames.removeAt(j);
                this->quantities.removeAt(j);
            }
            else j++;
        }
    }
}
//! Hàm này có gọi removeDuplicates hay không
InventoryManager InventoryManager::merge(const InventoryManager &inv1,
                                         const InventoryManager &inv2)
{
    // TODO
    InventoryManager inv = InventoryManager();
    for (int i = 0; i < inv1.size(); i++)
    {
        inv.addProduct( inv1.getProductAttributes(i), inv1.getProductName(i), 
                                                      inv1.getProductQuantity(i));
    }

    for (int i = 0; i < inv2.size(); i++)
    {
        inv.addProduct( inv2.getProductAttributes(i), inv2.getProductName(i), 
                                                      inv2.getProductQuantity(i));
    }
    //inv.removeDuplicates();
    return inv;
}

void InventoryManager::split(InventoryManager &section1,
                             InventoryManager &section2,
                             double ratio) const
{
    // TODO
    double temp = ratio;
    temp = this->size()*temp;
    int scale = (int)temp;
    if (abs (temp - (int)temp) > 1e-6)
    {
        scale++;
    }
    for (int i = 0; i < section1.size(); i++)
    {
        section1.removeProduct(i);
    }
    for (int i = 0; i < section2.size(); i++)
    {
        section2.removeProduct(i);
    }
    section1 = InventoryManager();
    section2 = InventoryManager();

    for (int i = 0; i < this->size(); i++)
    {
        if (i < scale)
        {
            section1.addProduct(this->getProductAttributes(i), this->getProductName(i), 
                                                               this->getProductQuantity(i));
        }
        else section2.addProduct(this->getProductAttributes(i), this->getProductName(i), 
                                                                this->getProductQuantity(i));
    }
}

List2D<InventoryAttribute> InventoryManager::getAttributesMatrix() const
{
    // TODO
    return this->attributesMatrix;    
}

List1D<string> InventoryManager::getProductNames() const
{
    // TODO
    return this->productNames;   
}

List1D<int> InventoryManager::getQuantities() const
{
    // TODO
    return this->quantities;   
}

string InventoryManager::toString() const
{
    // TODO
    stringstream result;
    result << "InventoryManager[\n";
    result << "  AttributesMatrix: ";
    result << this->getAttributesMatrix().toString();
    result << ",\n";
    result << "  ProductNames: ";
    result << getProductNames().toString();
    result << ",\n";
    result << "  Quantities: ";
    result << getQuantities().toString();
    result << "\n\
]";

    return result.str();
}

int InventoryManager::quantityFromName(string name) const
{
    // TODO
    for (int i = 0; i < this->size(); i++)
    {
        if (this->getProductName(i) == name)
        {
            return this->getProductQuantity(i);
        }
    }
    return -1;    
}