#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <list>
#include <unordered_map>

template<typename T>
class UnionFindElement {
  public:
    UnionFindElement(T element);
    UnionFindElement(T element, UnionFindElement<T> * parent);

    /**
     * Find the ultimate grandparent of this element.
     */
    UnionFindElement<T> * find_parent();


    /**
     * Find the immediate parent of this element.
     */
    UnionFindElement<T> * parent() const { return _parent; };

    /**
     * This element contained here.
     */
    const T element() const { return _element; };

    bool operator==(const UnionFindElement<T> other) const;

  private:
    T _element;
    UnionFindElement<T> * _parent;
};

template<typename T>
class UnionFind {

  public:
    /**
     * Add an element.
     */
    void add_element(T element);

    /**
     * Adds an element that is similar to another.
     */
    void add_element(T element, T similar_to);

    /**
     * Find the ultimate grandparent of this element.
     */
    const UnionFindElement<T> * find_parent(T element);

    /**
     * Find the element represented by the ultimate grandparent of the given element.
     */
    const T find_element(T element);

    /**
     * Return the map of all pairs <T, UnionFindElement<T>>
     */
    const std::unordered_map<T, UnionFindElement<T>> all_elements() const { return _elements; };


  private:
    std::unordered_map<T, UnionFindElement<T>> _elements;
};


#endif /* UNION_FIND_H */
