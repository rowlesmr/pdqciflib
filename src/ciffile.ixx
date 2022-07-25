


#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <variant>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t
#include <format>
#include <algorithm>
#include <utility>

import util;


export module ciffile;

using dataname = std::string;
using DataValue = std::vector<std::string>;
using itemorder = std::variant<int, dataname>;

template< typename K, typename V>
using dict = std::unordered_map<K, V>;


void print(const std::pair<dataname, DataValue>& kv) {
	if (kv.second.size() > 1) {
		std::cout << kv.first << '\n';
		for (const auto& v : kv.second) {
			std::cout << '\t' << v << '\n';
		}
	}
	else {
		std::cout << kv.first << '\t' << kv.second[0] << '\n';
	}
}

template<typename T>
void print(const T& v) {
	std::cout << v << '\n';
}

template<typename T>
void print(const std::vector<T>& vs, bool newline = true) {
	for (auto& v : vs) {
		if (newline) { std::cout << v << '\n'; }
		else { std::cout << v << '\t'; }
	}
	if (!newline) { std::cout << '\n'; }
}



export namespace row::cifstr {

	using dataname = std::string;
	using itemorder = std::variant<int, dataname>;
	using blockname = std::string;

	template< typename K, typename V>
	using dict = std::unordered_map<K, V>;


	class DataValue {
	public:
		using size_type = typename std::vector<std::string>::size_type;
		using iterator = typename std::vector<std::string>::iterator;
		using const_iterator = typename std::vector<std::string>::const_iterator;
		using reverse_iterator = typename std::vector<std::string>::reverse_iterator;
		using const_reverse_iterator = typename std::vector<std::string>::const_reverse_iterator;
		using reference = typename std::vector<std::string>::reference;
		using const_reference = typename std::vector<std::string>::const_reference;

		using const_iterator_double = typename std::vector<double>::const_iterator;
		using const_reverse_iterator_double = typename std::vector<double>::const_reverse_iterator;
		using const_reference_double = typename std::vector<double>::const_reference;

	private:
		std::vector<std::string> m_strs{};
		mutable std::vector<double> m_dbls{};
		mutable std::vector<double> m_errs{};
		mutable bool m_isConverted{ false };
		
	public:
		DataValue() {}
		DataValue(std::string in) : m_strs({ std::move(in) }) {}
		DataValue(std::vector<std::string> in) : m_strs( std::move(in) ) {}
		DataValue(std::initializer_list<std::string> in) : m_strs( std::move(in) ) {}


		void convert() const {
			if (m_isConverted){
				return;
			}

			m_dbls.clear();
			m_errs.clear();
			m_dbls.reserve(m_strs.size());
			m_errs.reserve(m_strs.size());

			for (const auto& str : m_strs) {
				auto [val, err] = row::util::stode(str);
				m_dbls.push_back(val);
				m_errs.push_back(err);
			}

			m_isConverted = true;
			return;
		}


		void reconvert() {
			m_isConverted = false;
			return;
		}


		constexpr void assign(size_type count, const std::string& value) {
			m_isConverted = false;
			m_strs.assign(count, value);
			return;
		}

		//element access
		constexpr reference at(size_type pos) noexcept(false) {
			return m_strs.at(pos);
		}
		constexpr const_reference at(size_type pos) const noexcept(false) {
			return m_strs.at(pos);
		}
		constexpr reference str_at(size_type pos) noexcept(false) {
			return at(pos);
		}
		constexpr const_reference str_at(size_type pos) const noexcept(false) {
			return at(pos);
		}
		const_reference_double dbl_at(size_type pos) const noexcept(false) {
			convert(); 
			return m_dbls.at(pos);
		}
		const_reference_double err_at(size_type pos) const noexcept(false){
			convert();
			return m_errs.at(pos);
		}

		constexpr reference front() { return m_strs.front(); }
		constexpr const_reference front() const { return m_strs.front(); }
		constexpr reference front_str() { return front(); }
		constexpr const_reference front_str() const { return front(); }
		const_reference_double front_dbl() const { convert(); return m_dbls.front(); }
		const_reference_double front_err() const { convert();  return m_errs.front(); }

		constexpr reference back() { return m_strs.back(); }
		constexpr const_reference back() const { return m_strs.back(); }
		constexpr reference back_str() { return back(); }
		constexpr const_reference back_str() const { return back(); }
		const_reference_double back_dbl() const { convert(); return m_dbls.back(); }
		const_reference_double back_err() const { convert(); return m_errs.back(); }


		constexpr std::string* data() noexcept {
			return m_strs.data();
		}
		constexpr const std::string* data() const noexcept {
			return m_strs.data();
		}
		constexpr std::string* str_data() noexcept {
			return data();
		}
		constexpr const std::string* str_data() const noexcept {
			return data();
		}
		const double* dbl_data() const noexcept {
			convert();
			return m_dbls.data();
		}
		const double* err_data() const noexcept {
			convert();
			return m_errs.data();
		}

		//iterators
		iterator begin() { return m_strs.begin(); };
		iterator end() { return m_strs.end(); };
		reverse_iterator rbegin() { return m_strs.rbegin(); };
		reverse_iterator rend() { return m_strs.rend(); };

		const_iterator begin() const { return m_strs.begin(); };
		const_iterator end() const { return m_strs.end(); };
		const_iterator cbegin() const { return m_strs.cbegin(); };
		const_iterator cend() const { return m_strs.cend(); };
		const_reverse_iterator crbegin() const { return m_strs.crbegin(); };
		const_reverse_iterator crend() const { return m_strs.crend(); };

		iterator str_begin() { return begin(); };
		iterator str_end() { return end(); };
		reverse_iterator str_rbegin() { return rbegin(); };
		reverse_iterator str_rend() { return rend(); };

		const_iterator str_begin() const { return begin(); };
		const_iterator str_end() const { return end(); };
		const_iterator str_cbegin() const { return cbegin(); };
		const_iterator str_cend() const { return cend(); };
		const_reverse_iterator str_crbegin() const { return crbegin(); };
		const_reverse_iterator str_crend() const { return crend(); };

		const_iterator_double dbl_begin() const { convert(); return m_dbls.begin(); };
		const_iterator_double dbl_end() const { convert(); return m_dbls.end(); };
		const_iterator_double dbl_cbegin() const { convert(); return m_dbls.cbegin(); };
		const_iterator_double dbl_cend() const { convert(); return m_dbls.cend(); };
		const_reverse_iterator_double dbl_crbegin() const { convert(); return m_dbls.crbegin(); };
		const_reverse_iterator_double dbl_crend() const { convert(); return m_dbls.crend(); };

		const_iterator_double err_begin() const { convert(); return m_errs.begin(); };
		const_iterator_double err_end() const { convert(); return m_errs.end(); };
		const_iterator_double err_cbegin() const { convert(); return m_errs.cbegin(); };
		const_iterator_double err_cend() const { convert(); return m_errs.cend(); };
		const_reverse_iterator_double err_crbegin() const { convert(); return m_errs.crbegin(); };
		const_reverse_iterator_double err_crend() const { convert(); return m_errs.crend(); };


		//capacity
		[[nodiscard]] constexpr bool empty() const noexcept {
			return m_strs.empty();
		}
		[[nodiscard]] constexpr bool isEmpty() const noexcept {
			return empty();
		}
		constexpr size_type size() const noexcept {
			return m_strs.size();
		}
		constexpr void reserve(size_type new_cap) {
			m_strs.reserve(new_cap);
			m_dbls.reserve(new_cap);
			m_errs.reserve(new_cap);
		}
		constexpr size_type capacity() const noexcept {
			return m_strs.capacity();
		}
		constexpr void shrink_to_fit() {
			m_strs.shrink_to_fit();
			m_dbls.shrink_to_fit();
			m_errs.shrink_to_fit();
		}

		//modifiers
		constexpr void clear() noexcept {
			m_strs.clear();
			m_dbls.clear();
			m_errs.clear();
			m_isConverted = false;
			return;
		}
		void push_back(const std::string& value) {
			m_isConverted = false;
			m_strs.push_back(value);
			return;
		}
		void push_back(std::string&& value) {
			m_isConverted = false;
			m_strs.push_back(std::forward<std::string>(value));
			return;
		}
		reference emplace_back(std::string&& value) {
			m_isConverted = false;
			return m_strs.emplace_back(std::forward<std::string>(value));
		}
		constexpr void swap(DataValue& other) {
			m_strs.swap(other.m_strs);
			m_dbls.swap(other.m_dbls);
			m_errs.swap(other.m_errs);
			m_isConverted = other.m_isConverted;
			return;
		}


		//"non"-member functions
		friend constexpr bool operator==(const DataValue& lhs, const DataValue& rhs) {
			return lhs.m_strs == rhs.m_strs;
		}
		friend constexpr auto operator<=>(const DataValue& lhs, const DataValue& rhs) {
			return lhs.m_strs <=> rhs.m_strs;
		}
		friend constexpr void swap(DataValue& lhs, DataValue& rhs) noexcept(noexcept(lhs.swap(rhs))) {
			lhs.swap(rhs);
		}
	};


	class Block {
	private:
		dict<dataname, DataValue> m_block{}; // this is the actual data
		dict<int, std::vector<dataname>> m_loops{}; // keeps track of datanames that are looped together
		std::vector<itemorder> m_item_order{}; // keeps the insertion order
		dict<dataname, dataname> m_true_case{}; // keeps the actual case of the keys used.

	public:
		bool overwrite{ true };

		//these print_* functions are for debugging purposes
		void print_block() const {
			std::cout << "---\nBlock:\n";
			for (auto& [k, vs] : m_block) {
				std::cout << k << ':';
				for (auto& v : vs) {
					std::cout << '\t' << v;
				}
				std::cout << '\n';
			}
			std::cout << "---\n";
		}

		void print_loops() const {
			std::cout << "---\nLoops:\n";
			for (auto& [k, vs] : m_loops) {
				std::cout << k << ':';
				for (auto& v : vs) {
					std::cout << '\t' << v;
				}
				std::cout << '\n';
			}
			std::cout << "---\n";
		}

		void print_item_order() const {
			std::cout << "---\nItem order:\n";
			for (auto& item : m_item_order) {
				if (item.index() == 0) {
					std::cout << std::get<int>(item) << '\t';
				}
				else {
					std::cout << std::get<std::string>(item) << '\t';
				}
			}
			std::cout << "\n---\n";
		}

		void print_true_case() const {
			std::cout << "---\nTrue case:\n";
			for (auto& [k, v] : m_true_case) {
				std::cout << k << " : " << v << ", ";
			}
			std::cout << "---\n";
		}



	public:
		row::util::Success addItem(const dataname& key, const DataValue& value) {
			dataname lowerKey{ row::util::toLower(key) };

			if (!overwrite && this->contains(lowerKey)) {
				return row::util::Success(1);
			}

			if (!this->contains(lowerKey) && !this->isInLoop(lowerKey)) {
				m_item_order.push_back(lowerKey);
			}

			if (m_true_case.contains(lowerKey)) {
				m_true_case.erase(lowerKey);
			}
			m_true_case[lowerKey] = key;
			m_block[lowerKey] = value;
			return row::util::Success();
		}

		row::util::Success addItems(std::vector<dataname>& keys, std::vector<DataValue>& values) {
			if (keys.size() != values.size()) {
				return row::util::Success(2);
			}

			for (size_t i = 0; i < keys.size(); ++i) {
				row::util::Success s = addItem(keys[i], values[i]);
				if (!s) {
					return s;
				}
			}
			return row::util::Success();
		}

		row::util::Success createLoop(const std::vector<dataname>& keys) {
			const auto& blk = m_block;
			std::vector<dataname> lowerKeys{ row::util::toLower(keys) };

			//check that all keys exist, and have all the same length values
			if (!(std::all_of(lowerKeys.cbegin(), lowerKeys.cend(), [&blk](const auto& key) { return blk.contains(key); }))) {
				return row::util::Success(3);
			}

			size_t len{ m_block[keys[0]].size() };
			if (!(std::all_of(lowerKeys.cbegin(), lowerKeys.cend(), [&blk, len](const auto& key) { return blk.at(key).size() == len; }))) {
				return row::util::Success(4);
			}

			//remove all keys from any existing loops
			for (auto& [_, loopKeys] : m_loops) {
				for (auto& key : lowerKeys) {
					if (row::util::contains(loopKeys, key)) {
						auto it = std::find(loopKeys.cbegin(), loopKeys.cend(), key);
						loopKeys.erase(it);
					}
				}
			}

			//remove empty loops
			for (auto& [loopNum, loopKeys] : m_loops) {
				if (loopKeys.size() == 0) {
					m_loops.erase(loopNum);
				}
			}

			//get the loop number to use for the loop we're about to insert
			int loopNum{ 1 };
			if (m_loops.size() > 0) {
				for (auto& [k, v] : m_loops) {
					if (k > loopNum)
						loopNum = k + 1;
				}
			}
			m_loops[loopNum] = lowerKeys;
			m_item_order.emplace_back(loopNum);

			//remove keys from item order
			for (auto& key : m_loops[loopNum]) {
				auto it = m_item_order.begin();
				while (it != m_item_order.end()) {
					if (it->index() == 0) {
						++it;
					}
					else if (std::get<std::string>(*it) == key) {
						it = m_item_order.erase(it);
					}
					else {
						++it;
					}
				}
			}
			return row::util::Success();
		}


		row::util::Success addNameToLoop(const dataname& newName, const dataname& oldName) {
			dataname lowerNew{ row::util::toLower(newName) };
			dataname lowerOld{ row::util::toLower(oldName) };

			if (!contains(lowerNew)) { return row::util::Success(3); }
			if (m_block.at(lowerNew).size() != m_block.at(lowerOld).size()) { return row::util::Success(4); }

			int loopNum = findLoop(oldName);
			if (loopNum < 0) { return row::util::Success(5); }
			if (row::util::contains(m_loops.at(loopNum), lowerNew)) { return row::util::Success(-1); }

			// if we get to here, the newName exists, the oldName is in a loop,  newName isn't in the oldName loop, and newName length is the same as oldName's

			//remove from other loops
			for (auto& [_, v] : m_loops) {
				std::erase(v, lowerNew);
			}
			m_loops[loopNum].push_back(lowerNew);
			std::erase(m_item_order, itemorder{ lowerNew });

			return row::util::Success();

		}


		int findLoop(const std::string& key) const {
			dataname lowerKey{ row::util::toLower(key) };

			for (auto& [k, v] : m_loops) {
				if (row::util::contains(v, lowerKey)) {
					return k;
				}
			}
			return -1;
		}

		row::util::Success getLoopNames(const dataname& key, std::vector<dataname>& keys) const {
			for (auto& [_, vs] : m_loops) {
				if (row::util::contains(vs, key)) {
					keys = vs;
					return row::util::Success();
				}
			}
			return row::util::Success(5);
		}

		row::util::Success isInLoop(const dataname& key) const {
			int r = findLoop(key);

			if (r != -1) {
				return row::util::Success();
			}
			else {
				return row::util::Success(5);
			}
		}


		row::util::Success removeItem(const dataname& key) {
			dataname lowerKey{ row::util::toLower(key) };

			if (!contains(lowerKey)) {
				return row::util::Success(6);
			}

			int loopNum = findLoop(key);
			m_block.erase(lowerKey);
			m_true_case.erase(lowerKey);

			if (loopNum > 0) {
				std::erase(m_loops[loopNum], lowerKey);

				if (m_loops[loopNum].size() == 0) {
					m_loops.erase(loopNum);
					std::erase_if(m_item_order, [loopNum](const auto& thing) { if (thing.index() == 0) { return std::get<int>(thing) == loopNum; } else { return false; }});
				}
			}
			else {
				std::erase_if(m_item_order, [lowerKey](const auto& thing) { if (thing.index() == 1) { return std::get<std::string>(thing) == lowerKey; } else { return false; }});
			}
			return row::util::Success();
		}


		std::tuple<int, int> getItemPosition(const dataname& key) const {
			/*A utility function to get the numerical order in the printout
		of `key`.  An item has coordinate `(loop_no,pos)` with
		the top level having a `loop_no` of -1.
		Return -1, -1, indicates doesn't exist.
		*/
			dataname lowerKey{ row::util::toLower(key) };

			if (!contains(lowerKey)) {
				return std::tuple<int, int> {-1, -1};
			}
			itemorder tmp{ lowerKey };
			if (row::util::contains(m_item_order, tmp)) {
				return std::tuple<int, int> {-1, row::util::getIndexOf(m_item_order, tmp)};
			}
			else {
				int loopNum = findLoop(lowerKey);
				auto it = std::find(m_loops.at(loopNum).cbegin(), m_loops.at(loopNum).cend(), lowerKey);
				int loopPos = it - m_loops.at(loopNum).cbegin();
				return std::tuple<int, int> {loopNum, loopPos};
			}
		}


		row::util::Success changeItemPosition(const dataname& key, const size_t newPosn) {
			/*Move the printout order of `key` to `newpos`. If `key` is
				in a loop, `newpos` refers to the order within the loop.*/
			dataname lowerKey{ row::util::toLower(key) };
			if (!contains(lowerKey)) {
				return row::util::Success(6);
			}
			auto [loopNum, oldPosn] = getItemPosition(key);
			if (loopNum < 0) {
				row::util::move_element(m_item_order, oldPosn, newPosn);
			}
			else {
				row::util::move_element(m_loops[loopNum], oldPosn, newPosn);
			}
			return row::util::Success();
		}

		row::util::Success changeLoopPosition(const dataname& key, const size_t newPosn) {
			/*Move the printout order of the loop containing `key` to `newpos`.*/
			dataname lowerKey{ row::util::toLower(key) };
			int loopNum = findLoop(key);
			if (loopNum < 0) {
				return row::util::Success(5);
			}
			else {
				itemorder tmp{ loopNum };
				row::util::move_element(m_item_order, row::util::getIndexOf(m_item_order, tmp), newPosn);
			}
			return row::util::Success();
		}


		void print() {
			std::cout << "---\n";
			for (const auto& item : m_item_order) {
				if (item.index() == 0) {
					int loopNum = std::get<int>(item);
					for (const auto& key : m_loops[loopNum]) {
						std::cout << '\t' << key << '\n';
					}

					for (size_t i{ 0 }; i < m_block[m_loops[loopNum][0]].size(); ++i) {
						for (const auto& key : m_loops[loopNum]) {
							std::cout << '\t' << m_block[key].at(i);
						}
						std::cout << '\n';
					}
				}
				else {
					std::string loopKey = std::get<std::string>(item);
					std::cout << loopKey << '\t' << m_block[loopKey].at(0) << '\n';
				}
			}
			std::cout << "---\n";
		}


		std::string printsection() {
			size_t maxKeyLen{};
			for (auto& item : m_item_order) {
				if (item.index() == 0) {
					continue;
				}
				maxKeyLen = std::max(maxKeyLen, std::get<dataname>(item).size());
			}


			return "";
		}

		std::string printLoop(int loopNum) const {
			const std::vector<dataname>& keys = m_loops.at(loopNum);
			std::string loop{ "loop_\n" };
			for (auto& key : m_loops.at(loopNum)) {
				loop += "  " + key;
			}

			std::vector<size_t> lens(keys.size(), 0);
			for (size_t i{ 0 }; i < lens.size(); ++i) {
				const dataname& key = keys[i];
				const DataValue& values = m_block.at(key);
				for (auto& val : values) {
					lens[i] = std::max(lens[i], val.size());
				}
			}

			size_t maxRow = m_block.at(keys.at(0)).size();
			size_t maxCol = keys.size();
			for (size_t row{ 0 }; row < maxRow; ++row) {
				for (size_t col{ 0 }; col < maxCol; ++col) {
					const std::string& value = m_block.at(m_loops.at(loopNum)[col]).at(row);
					const int pad = lens[col];
					loop += std::format("\t{:{}}", pad, value);
				}
				loop += '\n';
			}


			return loop;
		}



		//// taken from https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
		//struct Iterator
		//{
		//	using iterator_category = std::forward_iterator_tag;
		//	using difference_type = std::ptrdiff_t;
		//	using value_type = std::pair<const dataname, datavalue>;
		//	using pointer = value_type*;
		//	using reference = value_type&;

		//	Iterator(pointer m_ptr, Block* blk)
		//		: m_ptr{ m_ptr }, block{ blk } { }
		//	
		//	reference operator*() const { return *m_ptr; }
		//	pointer operator->() { return m_ptr; }

		//	// Prefix increment
		//	Iterator& operator++() { 
		//		dataname currentKey{ m_ptr->first };
		//		auto [loopNum, posn] = block->getItemPosition(currentKey);
		//		itemorder nextItem{};
		//		if (loopNum < 0) { // currentKey is not a loop
		//			if (posn < block->m_item_order.size() - 1) { // not at the end of the item_order
		//				nextItem = block->m_item_order[++posn];
		//				if (nextItem.index() == 0) { // the nextkey is the start of a loop
		//					currentKey = block->m_loops.at(std::get<int>(nextItem))[0];//update the keyname
		//				}
		//				else { //the next key is just a key
		//					currentKey = std::get<dataname>(nextItem); //update the keyname
		//				}
		//				m_ptr = &(*block->m_block.find(currentKey));
		//			}
		//			else {
		//				m_ptr = nullptr;
		//			}
		//		}
		//		else {  //we're in a loop
		//			if (posn < block->m_loops[loopNum].size() - 1) { // not at the end of the loop
		//				currentKey = block->m_loops[loopNum][++posn];
		//				m_ptr = &(*block->m_block.find(currentKey));
		//			}
		//			else { //we're at the end of a loop and need to look at the next element
		//				int currentIndex = row::util::getIndexOf(block->m_item_order, std::variant<int, dataname>{loopNum});
		//				if (currentIndex < block->m_item_order.size() - 1) { // we can go to the next element
		//					nextItem = block->m_item_order[++currentIndex];
		//					if (nextItem.index() == 0) { // the nextkey is the start of a loop
		//						currentKey = block->m_loops.at(std::get<int>(nextItem))[0];//update the keyname
		//					}
		//					else { //the next key is just a key
		//						currentKey = std::get<dataname>(nextItem); //update the keyname
		//					}
		//					m_ptr = &(*block->m_block.find(currentKey));
		//				}
		//				else {
		//					m_ptr = nullptr;
		//				}
		//			}
		//		}
		//		return *this; 
		//	}

		//	// Postfix increment
		//	Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		//	friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		//	friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

		//private:
		//	pointer m_ptr;
		//	Block* block;
		//};


		struct ConstIterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = std::pair<const dataname, DataValue>;
			using pointer = value_type*;
			using reference = value_type&;
			using const_pointer = const std::pair<const dataname, DataValue>*;
			using const_reference = const std::pair<const dataname, DataValue>&;

			ConstIterator(const_pointer m_ptr, const Block* blk)
				: m_ptr{ m_ptr }, block{ blk } { }

			const_reference operator*() const { return *m_ptr; }
			const_pointer operator->() { return m_ptr; }

			// Prefix increment
			ConstIterator& operator++() {
				dataname currentKey{ m_ptr->first };
				auto [loopNum, posn] = block->getItemPosition(currentKey);
				itemorder nextItem{};
				if (loopNum < 0) { // currentKey is not a loop
					if (posn < block->m_item_order.size() - 1) { // not at the end of the item_order
						nextItem = block->m_item_order[++posn];
						if (nextItem.index() == 0) { // the nextkey is the start of a loop
							currentKey = block->m_loops.at(std::get<int>(nextItem))[0];//update the keyname
						}
						else { //the next key is just a key
							currentKey = std::get<dataname>(nextItem); //update the keyname
						}
						m_ptr = &(*block->m_block.find(currentKey));
					}
					else {
						m_ptr = nullptr;
					}
				}
				else {  //we're in a loop
					if (posn < block->m_loops.at(loopNum).size() - 1) { // not at the end of the loop
						currentKey = block->m_loops.at(loopNum)[++posn];
						m_ptr = const_cast<pointer>(&(*block->m_block.find(currentKey)));
					}
					else { //we're at the end of a loop and need to look at the next element
						int currentIndex = row::util::getIndexOf(block->m_item_order, std::variant<int, dataname>{ loopNum });
						if (currentIndex < block->m_item_order.size() - 1) { // we can go to the next element
							nextItem = block->m_item_order[++currentIndex];
							if (nextItem.index() == 0) { // the nextkey is the start of a loop
								currentKey = block->m_loops.at(std::get<int>(nextItem))[0];//update the keyname
							}
							else { //the next key is just a key
								currentKey = std::get<dataname>(nextItem); //update the keyname
							}
							m_ptr = const_cast<pointer>(&(*block->m_block.find(currentKey)));
						}
						else {
							m_ptr = nullptr;
						}
					}
				}
				return *this;
			}

			// Postfix increment
			ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr == b.m_ptr; };
			friend bool operator!= (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr != b.m_ptr; };


		private:
			const_pointer m_ptr;
			const Block* block;
		};



		//Iterators
		//Iterator begin() noexcept {
		//	return Iterator(ptrToFirstItem(), this);
		//}
		//Iterator end() noexcept {
		//	return Iterator(nullptr, this);
		//}

		ConstIterator begin() const noexcept {
			return ConstIterator(constptrToFirstItem(), this);
		}
		ConstIterator end() const noexcept {
			return ConstIterator(nullptr, this);
		}

		ConstIterator cbegin() const noexcept {
			return ConstIterator(constptrToFirstItem(), this);
		}
		ConstIterator cend() const noexcept {
			return ConstIterator(nullptr, this);
		}

		//capacity
		[[nodiscard]] bool empty() const noexcept {
			return m_block.empty();
		}
		[[nodiscard]] bool isEmpty() const noexcept {
			return empty();
		}
		size_t size() const noexcept {
			return m_block.size();
		}
		size_t size(const dataname& key) const noexcept {
			if (contains(key)) {
				return m_block.at(key).size();
			}
			return -1;
		}
		size_t max_size() const noexcept {
			return std::min({ m_block.max_size(), m_item_order.max_size(), m_loops.max_size(), m_true_case.max_size() });
		}

		// Modifiers
		void clear() noexcept {
			m_block.clear();
			m_loops.clear();
			m_item_order.clear();
			m_true_case.clear();
			overwrite = true;
		}
		size_t erase(const dataname& key) {
			row::util::Success s = removeItem(key);
			if (s) { return 1; }
			else { return 0; }
		}
		row::util::Success set(const dataname& key, const DataValue& value) {
			return addItem(key, value);
		}
		row::util::Success put(const dataname& key, const DataValue& value) {
			return set(key, value);
		}


		// Lookup
		row::util::Success get(const dataname& key, DataValue& value) const {
			if (this->contains(key)) {
				value = m_block.at(key);
				return row::util::Success();
			}
			return row::util::Success(6);
		}
		const DataValue& at(const dataname& key) const {
			return m_block.at(key);
		}
		size_t count(const dataname& key) const {
			return m_block.count(key);
		}
		ConstIterator find(const dataname& key) const {
			std::pair<const dataname, DataValue>* m_ptr = const_cast<std::pair<const dataname, DataValue>*>(&(*m_block.find(key)));
			return ConstIterator(m_ptr, this);
		}
		bool contains(const dataname& key) const {
			return m_block.contains(key);
		}

		// see https://stackoverflow.com/a/73088991/36061 and https://stackoverflow.com/questions/19217813/operator-c-get-set
		struct Result {
			Block& map_;
			dataname key_;
			Result(Block& m, dataname k) : map_(m), key_(k) {}
			operator DataValue () const { return map_.get_item_(key_); }
			DataValue& operator = (DataValue rhs) {
				return map_.set_item_(key_, rhs);
			}
		};
		Result operator[](const dataname& key) {
			return Result(*this, key);
		}
		DataValue operator[](dataname& key) const {
			return get_item_(key);
		}



	private:

		std::pair<const dataname, DataValue>* ptrToFirstItem() {
			itemorder firstItem = m_item_order[0];
			dataname firstKey{};
			if (firstItem.index() == 0) {
				int loopNum = std::get<int>(firstItem);
				firstKey = m_loops.at(loopNum)[0];
			}
			else {
				firstKey = std::get<dataname>(firstItem);
			}
			return &(*m_block.find(firstKey));
		}

		std::pair<const dataname, DataValue>* constptrToFirstItem() const {
			itemorder firstItem = m_item_order[0];
			dataname firstKey{};
			if (firstItem.index() == 0) {
				int loopNum = std::get<int>(firstItem);
				firstKey = m_loops.at(loopNum)[0];
			}
			else {
				firstKey = std::get<dataname>(firstItem);
			}
			std::pair<const dataname, DataValue>* ptr = const_cast<std::pair<const dataname, DataValue>*>(&(*m_block.find(firstKey)));
			return ptr;
		}

		DataValue get_item_(const dataname& key) const {
			dataname lowerKey{ row::util::toLower(key) };
			return m_block.at(lowerKey);
		}
		DataValue& set_item_(dataname key, DataValue value) {
			addItem(key, value);
			dataname lowerKey{ row::util::toLower(key) };
			return m_block[lowerKey];
		}

	};

	class Cif {
	private:
		dict<blockname, Block> m_cif{}; // this is the actual data
		std::vector<blockname> m_block_order{}; // keeps the insertion order
		dict<blockname, blockname> m_true_case{}; // keeps the actual case of the blocknames used.

	public:
		bool overwrite{ true };

		//these print_* functions are for debugging purposes
		void print_cif() const {
			std::cout << "---\nCIF keys:\n";
			for (auto& [k, _] : m_cif) {
				std::cout << k << '\n';
			}
			std::cout << "---\n";
		}

		void print_block_order() const {
			std::cout << "---\nBlock order:\n";
			for (auto& block : m_block_order) {
				std::cout << block << '\n';
			}
			std::cout << "---\n";
		}

		void print_true_case() const {
			std::cout << "---\nTrue case:\n";
			for (auto& [k, v] : m_true_case) {
				std::cout << k << " : " << v << ", ";
			}
			std::cout << "---\n";
		}


	public:
		row::util::Success addBlock(const blockname& key, const Block& block) {
			dataname lowerKey{ row::util::toLower(key) };

			if (!overwrite && contains(lowerKey)) {
				return row::util::Success(1);
			}

			if (!contains(lowerKey)) {
				m_block_order.push_back(lowerKey);
			}

			m_true_case.erase(lowerKey);
			m_true_case[lowerKey] = key;
			m_cif[lowerKey] = block;
			return row::util::Success();
		}


		row::util::Success addBlocks(std::vector<blockname>& keys, std::vector<Block>& blocks) {
			if (keys.size() != blocks.size()) {
				return row::util::Success(2);
			}

			for (size_t i = 0; i < keys.size(); ++i) {
				row::util::Success s = addBlock(keys[i], blocks[i]);
				if (!s) {
					return s;
				}
			}
			return row::util::Success();
		}


		row::util::Success removeBlock(const blockname& key) {
			dataname lowerKey{ row::util::toLower(key) };

			if (!contains(lowerKey)) {
				return row::util::Success(6);
			}

			m_cif.erase(lowerKey);
			m_true_case.erase(lowerKey);
			std::erase_if(m_block_order, [lowerKey](const auto& thing) { return thing == lowerKey; });

			return row::util::Success();
		}


		int getBlockPosition(const dataname& key) const {
			/*A utility function to get the numerical order in the printout
		of `key`.  An item has coordinate `(loop_no,pos)` with
		the top level having a `loop_no` of -1.
		Return -1, -1, indicates doesn't exist.
		*/
			dataname lowerKey{ row::util::toLower(key) };

			if (!contains(lowerKey)) {
				return -1;
			}
			if (row::util::contains(m_block_order, lowerKey)) {
				return row::util::getIndexOf(m_block_order, lowerKey);
			}
		}


		row::util::Success changeBlockPosition(const dataname& key, const size_t newPosn) {
			/*Move the printout order of `key` to `newpos`. If `key` is
				in a loop, `newpos` refers to the order within the loop.*/
			dataname lowerKey{ row::util::toLower(key) };
			if (!contains(lowerKey)) {
				return row::util::Success(6);
			}
			int oldPosn = getBlockPosition(key);
			row::util::move_element(m_block_order, oldPosn, newPosn);

			return row::util::Success();
		}


		void print() {
			std::cout << "---\n";
			for (const auto& block : m_block_order) {
				std::cout << block << '\n';
				Block blk = m_cif.at(block); //this is here to throw
			}
			std::cout << "---\n";
		}



		//// taken from https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
		//struct Iterator
		//{
		//	using iterator_category = std::forward_iterator_tag;
		//	using difference_type = std::ptrdiff_t;
		//	using value_type = std::pair<const blockname, Block>;
		//	using pointer = value_type*;
		//	using reference = value_type&;

		//	Iterator(pointer m_ptr, Cif* cif)
		//		: m_ptr{ m_ptr }, cif{ cif } { }

		//	reference operator*() const { return *m_ptr; }
		//	pointer operator->() { return m_ptr; }

		//	// Prefix increment
		//	Iterator& operator++() {
		//		blockname currentBlock{ m_ptr->first };
		//		int posn = cif->getBlockPosition(currentBlock);
		//		if (posn < cif->m_block_order.size() - 1) { // not at the end of the block_order
		//			currentBlock = cif->m_block_order[++posn]; //update the current block to the next one
		//			m_ptr = &(*cif->m_cif.find(currentBlock));
		//		}
		//		else {
		//			m_ptr = nullptr;
		//		}
		//		return *this;
		//	}

		//	// Postfix increment
		//	Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		//	friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		//	friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

		//private:
		//	pointer m_ptr;
		//	Cif* cif;
		//};


		struct ConstIterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = std::pair<const blockname, Block>;
			using pointer = value_type*;
			using reference = value_type&;
			using const_pointer = const std::pair<const blockname, Block>*;
			using const_reference = const std::pair<const blockname, Block>&;

			ConstIterator(const_pointer m_ptr, const Cif* cif)
				: m_ptr{ m_ptr }, cif{ cif } { }

			const_reference operator*() const { return *m_ptr; }
			const_pointer operator->() { return m_ptr; }

			// Prefix increment
			ConstIterator& operator++() {
				blockname currentBlock{ m_ptr->first };
				int posn = cif->getBlockPosition(currentBlock);
				if (posn < cif->m_block_order.size() - 1) { // not at the end of the block_order
					currentBlock = cif->m_block_order[++posn]; //update the current block to the next one
					m_ptr = &(*cif->m_cif.find(currentBlock));
				}
				else {
					m_ptr = nullptr;
				}
				return *this;
			}

			// Postfix increment
			ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr == b.m_ptr; };
			friend bool operator!= (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr != b.m_ptr; };


		private:
			const_pointer m_ptr;
			const Cif* cif;
		};



		//Iterators
		//Iterator begin() noexcept {
		//	return Iterator(ptrToFirstItem(), this);
		//}
		//Iterator end() noexcept {
		//	return Iterator(nullptr, this);
		//}

		ConstIterator begin() const noexcept {
			return ConstIterator(constptrToFirstBlock(), this);
		}
		ConstIterator end() const noexcept {
			return ConstIterator(nullptr, this);
		}

		ConstIterator cbegin() const noexcept {
			return ConstIterator(constptrToFirstBlock(), this);
		}
		ConstIterator cend() const noexcept {
			return ConstIterator(nullptr, this);
		}

		//capacity
		[[nodiscard]] bool empty() const noexcept {
			return m_cif.empty();
		}
		[[nodiscard]] bool isEmpty() const noexcept {
			return empty();
		}
		size_t size() const noexcept {
			return m_cif.size();
		}
		size_t size(const std::string& key) const noexcept {
			if (contains(key)) {
				return m_cif.at(key).size();
			}
			return -1;
		}
		size_t max_size() const noexcept {
			return std::min({ m_cif.max_size(), m_block_order.max_size(), m_true_case.max_size() });
		}

		// Modifiers
		void clear() noexcept {
			m_cif.clear();
			m_block_order.clear();
			m_true_case.clear();
			overwrite = true;
		}
		size_t erase(const blockname& key) {
			row::util::Success s = removeBlock(key);
			if (s) { return 1; }
			else { return 0; }
		}
		row::util::Success set(const blockname& key, const Block& value) {
			return addBlock(key, value);
		}
		row::util::Success put(const blockname& key, const Block& value) {
			return set(key, value);
		}


		// Lookup
		row::util::Success get(const blockname& key, Block& value) const {
			if (this->contains(key)) {
				value = m_cif.at(key);
				return row::util::Success();
			}
			return row::util::Success(6);
		}
		const Block& at(const blockname& key) const {
			return m_cif.at(key);
		}
		size_t count(const blockname& key) const {
			return m_cif.count(key);
		}
		ConstIterator find(const blockname& key) const {
			std::pair<const blockname, Block>* m_ptr = const_cast<std::pair<const blockname, Block>*>(&(*m_cif.find(key)));
			return ConstIterator(m_ptr, this);
		}
		bool contains(const blockname& key) const {
			return m_cif.contains(key);
		}

		// see https://stackoverflow.com/a/73088991/36061 and https://stackoverflow.com/questions/19217813/operator-c-get-set
		struct Result {
			Cif& map_;
			blockname key_;
			Result(Cif& m, blockname k) : map_(m), key_(k) {}
			operator Block () const { return map_.get_item_(key_); }
			Block& operator = (Block rhs) {
				return map_.set_item_(key_, rhs);
			}
		};
		Result operator[](const blockname& key) {
			return Result(*this, key);
		}
		Block operator[](const blockname& key) const {
			return get_item_(key);
		}



	private:

		std::pair<const blockname, Block>* ptrToFirstBlock() {
			return &(*m_cif.find(m_block_order[0]));
		}

		std::pair<const blockname, Block>* constptrToFirstBlock() const {
			std::pair<const blockname, Block>* ptr = const_cast<std::pair<const blockname, Block>*>(&(*m_cif.find(m_block_order[0])));
			return ptr;
		}

		Block get_item_(const blockname& key) const {
			dataname lowerKey{ row::util::toLower(key) };
			return m_cif.at(lowerKey);
		}
		Block& set_item_(blockname key, Block value) {
			addBlock(key, value);
			dataname lowerKey{ row::util::toLower(key) };
			return m_cif[lowerKey];
		}
	};





}







export void MyFunc();