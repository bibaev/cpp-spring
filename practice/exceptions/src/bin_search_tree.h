#pragma once
#include <memory>
#include <stdexcept>
#include <cassert>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <utility>

namespace details
{

template<class KEY, class VALUE>
struct node
{
    node(const KEY &key, const VALUE &val)
    {}

    ~node() noexcept
    {}

    node(node &&src) = default;
    node& operator=(node &&src) = default;

    node(const node &src) = delete;
    node& operator=(const node &src) = delete;

    void putLeft(std::unique_ptr<node> &&nptr) noexcept
    {
    }

    std::unique_ptr<node> takeLeft() noexcept
    {
    }

    void putRight(std::unique_ptr<node> &&nptr) noexcept
    {
    }

    std::unique_ptr<node> takeRight() noexcept
    {
    }

    node*& parent() noexcept
    {
    }

    node* left() noexcept
    {
    }

    node* right() noexcept
    {
    }

    KEY& key() noexcept
    {
    }

    VALUE &value() noexcept
    {
    }
};

} // namespace details

// TODO bst_key_exists_exception: std::logic_error
// TODO bst_key_absent_exception: std::logic_error

// KEY should have operator <, <<, () constructor
template<class KEY, class VALUE>
struct bin_search_tree
{
    bin_search_tree() = default;

    // throws bst_key_exists_exception
    void insert(const KEY &key, const VALUE &val);

    // throws bst_key_exists_exception
    template<class KIT, class VIT>
    void insert(KIT kbegin, KIT kend, VIT vbegin);

    // throws bst_key_absent_exception
    VALUE& find(const KEY &key);
private:
    typedef details::node<KEY, VALUE> node;
    node* find_nearest_node(const KEY &key);
    void reinsert_node(std::unique_ptr<node> &&node_ptr) noexcept;
    void erase_node(node *node) noexcept;
    static bool should_go_right(node *parent, const KEY &key);

    std::unique_ptr<node> root_ptr_;
};

template<class KEY, class VALUE>
auto bin_search_tree<KEY, VALUE>::find_nearest_node(const KEY &key) -> node*
{
    if (!root_ptr_)
        return nullptr;

    node *cur_node = root_ptr_.get();
    for(;;)
    {
        if (cur_node->key() == key)
            break;

        node *next_child = should_go_right(cur_node, key)
            ? cur_node->right()
            : cur_node->left();

        if (!next_child)
            break;
        cur_node = next_child;
    }
    assert(cur_node);
    return cur_node;
}

template<class KEY, class VALUE>
bool bin_search_tree<KEY, VALUE>::should_go_right(node *parent, const KEY &key) {
    return key > parent->key();
};

template<class KEY, class VALUE>
void bin_search_tree<KEY, VALUE>::erase_node(node *enode) noexcept
{
    node *parent = enode->parent();
    std::unique_ptr<node> left_ptr = enode->takeLeft();
    std::unique_ptr<node> right_ptr = enode->takeRight();

    if (parent)
    {
        if (should_go_right(parent, enode->key()))
        {
            parent->putRight(nullptr);
        }
        else
        {
            parent->putLeft(nullptr);
        }
    }
    else
    {
        root_ptr_ = nullptr;
    }
    reinsert_node(std::move(left_ptr));
    reinsert_node(std::move(right_ptr));
}
