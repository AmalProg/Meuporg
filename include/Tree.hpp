#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <vector>

template < class T > class Tree
{
    public:
        Tree(T * obj, Tree * parent) : c_Object(obj), c_Parent(parent)
        {}
        ~Tree()
        { for(uint16_t i = 0; i < c_Childrens.size(); i++)
            delete c_Childrens[i]; }

        void addChildren(T * obj)
        { c_Childrens.push_back(new Tree<T>(obj, this));}

        uint16_t getNbrChildrens() const { return c_Childrens.size(); }
        Tree * getChildren(uint16_t i) const { return c_Childrens[i]; }
        std::vector< Tree * > getCildrens() const { return c_Childrens; };
        T * getObject() const { return c_Object; }

    private:
        T * c_Object;
        std::vector< Tree * > c_Childrens;
        Tree * c_Parent;
};

#endif // TREE_HPP
