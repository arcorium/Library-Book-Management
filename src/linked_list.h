#pragma once

#include <optional>

// LINKED LIST
#include <cassert>
#include <iostream>

template <typename T>
struct Node
{
	T data;
	Node<T>* next;
};

template<typename T>
class LinkedList
{
public:
	LinkedList() : m_length{ 0 } {}
	~LinkedList()
	{
		auto current = m_head;
		while (current)
		{
			auto temp = current->next;
			delete current;
			current = temp;
		}
	}

	// Insert
	void push_back(T value_)
	{
		auto node = new Node<T>{ value_, nullptr };

		if (m_tail)
		{
			m_tail->next = node;
		}
		m_tail = node;
		if (!m_head)	m_head = node;

		++m_length;
	}

	void push_front(T value_)
	{
		auto node = new Node<T>{ value_, nullptr };

		if (m_head)
		{
			node->next = m_head;
		}
		m_head = node;
		if (!m_tail)	m_tail = node;

		++m_length;
	}

	void insert(size_t index_, T value_)
	{
		assert(index_ < m_length);

		auto node = new Node<T>{ value_, nullptr };

		// set head
		if (!index_)
		{
			push_front(value_);
		}
		// set tail
		else if (index_ == m_length)
		{
			push_back(value_);
		}
		// other
		else
		{
			auto before = get(index_ - 1);
			auto after = before->next;
			before->next = node;
			node->next = after;

			++m_length;
		}
	}

	// Delete
	Node<T>* pop()
	{
		if (!m_head)	return nullptr;

		auto temp = m_head;
		m_head = m_head->next;

		// last element
		if (temp == m_tail)	m_tail = m_head;

		--m_length;
		return temp;
	}

	Node<T>* pop_back()
	{
		if (!m_tail)	return nullptr;

		Node<T>* beforeTail{};
		if (m_head == m_tail)
		{
			beforeTail = nullptr;
		}
		else
		{
			beforeTail = get(m_length - 2);
			assert(beforeTail->next == m_tail);
		}

		beforeTail->next = nullptr;
		auto temp = m_tail;
		m_tail = beforeTail;

		if (temp == m_head)	m_head = m_tail;

		--m_length;
		return temp;
	}

	Node<T>* remove(size_t index_)
	{
		if (index_ >= m_length) return nullptr;

		// set head
		if (!index_)
		{
			return pop();
		}
		// set tail
		else if (index_ == m_length)
		{
			return pop_back();
		}
		// other
		else
		{
			auto before = get(index_ - 1);
			auto target = before->next;

			before->next = target->next;

			--m_length;

			return target;
		}
	}

	// get
	size_t length()
	{
		return m_length;
	}

	Node<T>* head()
	{
		return m_head;
	}

	Node<T>* tail()
	{
		return m_tail;
	}

	Node<T>* get(size_t index_)
	{
		if (index_ >= m_length)	return nullptr;
		if (index_ == m_length - 1) return m_tail;

		auto current = m_head;

		for (unsigned i = 0; i < index_; ++i)
		{
			current = current->next;
		}

		return current;
	}

	// concat
	void concat_back(const LinkedList<T>& list_)
	{
		m_tail->next = list_.head();
		m_tail = list_.tail();

		m_length += list_.length();
	}

	void concat_front(const LinkedList<T>& list_)
	{
		list_.m_tail->next = m_head;
		m_head = list_.head();

		m_length += list_.length();
	}

	void concat(size_t index_, const LinkedList<T>& list_)
	{
		assert(index_ < m_length);

		auto before = get(index_ - 1);
		auto after = before->next;

		before->next = list_.head();
		list_.m_tail->next = after;

		m_length += list_.length();
	}

private:
	Node<T>* m_head{};
	Node<T>* m_tail{};

	size_t m_length;
};

// END LINKED LIST

template <typename T>
class Queue
{
public:
	bool is_empty()
	{
		return !m_list.length();
	}

	void Enqueue(T value_)
	{
		m_list.push_back(value_);
	}

	std::optional<T> Dequeue()
	{
		if (is_empty())	return std::nullopt;

		return m_list.pop()->data;
	}

	void clear()
	{
		while (m_list.head())
		{
			m_list.pop();
		}
	}

	LinkedList<T>* data()
	{
		return &m_list;
	}

private:
	LinkedList<T> m_list;
};