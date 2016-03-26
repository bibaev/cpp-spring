#pragma once
#include <memory>
#include <stdexcept>
#include <cassert>
#include <iterator>
#include <vector>

namespace details
{

template<class KEY, class VALUE>
struct node {
    node(const KEY &key, const VALUE &val) 
		: key_(key)
		, value_(val)
		, parent_(nullptr)
	{}

    ~node() noexcept
    {}

    node(node &&src) = default;
    node& operator=(node &&src) = default;

    node(const node &src) = delete;
    node& operator=(const node &src) = delete;

    void putLeft(std::unique_ptr<node> &&nptr) noexcept
    {
		left_ = std::move(nptr);
    }

    std::unique_ptr<node> takeLeft() noexcept
    {
		return std::move(left_);
    }

    void putRight(std::unique_ptr<node> &&nptr) noexcept
    {
		right_ = std::move(nptr);
    }

    std::unique_ptr<node> takeRight() noexcept
    {
		return std::move(right_);
    }

    node*& parent() noexcept
    {
		return parent_;
    }

    node* left() noexcept
    {
		return left_.get();
    }

    node* right() noexcept
    {
		return right_.get();
    }

    KEY& key() noexcept
    {
		return key_;
    }

    VALUE &value() noexcept
    {
		return value_;
    }

private:
	KEY key_;
	VALUE value_;
	node* parent_;
	std::unique_ptr<node> left_;
	std::unique_ptr<node> right_;
};

} // namespace details

// TODO bst_key_exists_exception: std::logic_error
struct bst_key_exists_exception : std::logic_error {
	explicit bst_key_exists_exception(const std::string& message)
		: logic_error(message) {}

	explicit bst_key_exists_exception(const char* message)
		: logic_error(message) {}
};

// TODO bst_key_absent_exception: std::logic_error
struct bst_key_absent_exception: std::logic_error {
	explicit bst_key_absent_exception(const std::string& message)
		: logic_error(message) {}

	explicit bst_key_absent_exception(const char* message)
		: logic_error(message) {}
};


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
	bool contains(const KEY &key) noexcept;
private:
    typedef details::node<KEY, VALUE> node;
    node* find_nearest_node(const KEY &key);
    void insert_node(std::unique_ptr<node> &&node_ptr);
    void reinsert_node(std::unique_ptr<node> &&node_ptr) noexcept;
    void erase_node(node *node) noexcept;
    static bool should_go_right(node *parent, const KEY &key);

    std::unique_ptr<node> root_ptr_;
};

template <class KEY, class VALUE>
void bin_search_tree<KEY, VALUE>::insert(const KEY& key, const VALUE& val) {
	std::unique_ptr<node> new_node(new node(key, val));
	insert_node(std::move(new_node));
}

template <class KEY, class VALUE>
template <class KIT, class VIT>
void bin_search_tree<KEY, VALUE>::insert(KIT kbegin, KIT kend, VIT vbegin) {
	size_t count = std::distance(kbegin, kend);
	std::vector<std::unique_ptr<node>> new_nodes;

	auto kcur = kbegin;
	auto vcur = vbegin;
	size_t filled;
	for (filled = 0; filled < count; ++filled) {
		if (contains(*kcur)) {
			throw bst_key_exists_exception("Key already exists.");
		}

		new_nodes.emplace_back(new node(*kcur, *vcur));
		++kcur; ++vcur;
	}

	for (size_t i = 0; i < count; ++i) {
		insert_node(std::move(new_nodes[i]));
	}
}

template <class KEY, class VALUE>
VALUE& bin_search_tree<KEY, VALUE>::find(const KEY& key) {
	auto nearest = find_nearest_node(key);
	if (nearest == nullptr || nearest->key() != key) {
		throw bst_key_absent_exception("Key not found.");
	}

	return nearest->value();
}

template <class KEY, class VALUE>
bool bin_search_tree<KEY, VALUE>::contains(const KEY& key) noexcept {
	auto nearest = find_nearest_node(key);
	return nearest != nullptr && nearest->key() == key;
}

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

template <class KEY, class VALUE>
void bin_search_tree<KEY, VALUE>::insert_node(std::unique_ptr<node>&& node_ptr) {
	if (root_ptr_ == nullptr) {
		root_ptr_ = std::move(node_ptr);
		return;
	}

	auto current_node = root_ptr_.get();

	auto next_child = current_node;
	while (true) {
		next_child = should_go_right(current_node, node_ptr->key())
			? current_node->right()
			: current_node->left();
		if (next_child == nullptr) {
			break;
		}

		current_node = next_child;
	}

	node_ptr->parent() = current_node;
	if (should_go_right(current_node, node_ptr->key())) {
		current_node->putRight(std::move(node_ptr));
	}
	else {
		current_node->putLeft(std::move(node_ptr));
	}
}


template <class KEY, class VALUE>
void bin_search_tree<KEY, VALUE>::reinsert_node(std::unique_ptr<node>&& node_ptr) noexcept {
	try {
		insert_node(std::move(node_ptr));
	}
	catch(std::exception const& ex) {
		assert(false);
	}
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
