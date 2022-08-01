


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
import cifexcept;

export module ciffile;

export namespace row::cif {

	using dataname = std::string;
	template< typename K, typename V>
	using dict = std::unordered_map<K, V>;


	class Datavalue {
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
		Datavalue() {}
		Datavalue(std::string in) : m_strs({ std::move(in) }) {}
		Datavalue(std::vector<std::string> in) : m_strs( std::move(in) ) {}
		Datavalue(std::initializer_list<std::string> in) : m_strs{ std::move(in) } { std::cout << "here\n"; }

		bool convert() const {
			if (m_isConverted){
				return m_isConverted;
			}

			//test the first one. If it passes, assume the rest will.
			if (m_strs.size()) {
				auto [val, err] = row::util::stode(m_strs[0]);
				if (val == row::util::NaN && err == row::util::NaN) {
					m_isConverted = false;
					m_dbls.clear();
					m_errs.clear();
					return m_isConverted;
				}
			}
			else {
				m_isConverted = false;
				m_dbls.clear();
				m_errs.clear();
				return m_isConverted;
			}

			m_dbls.clear();
			m_errs.clear();
			m_dbls.reserve(m_strs.size());
			m_errs.reserve(m_strs.size());

			for (const auto& s : m_strs) {
				auto [val, err] = row::util::stode(s);
				this->m_dbls.push_back(val);
				this->m_errs.push_back(err);
			}

			m_isConverted = true;
			return m_isConverted;
		}

		void reconvert() {
			m_isConverted = false;
			return;
		}

		bool isConverted() const {
			return m_isConverted;
		}


		//vector access
		const std::vector<std::string>& getStrings() const {
			return m_strs;
		}
		const std::vector<double>& getDoubles() const {
			convert();
			return m_dbls;
		}
		const std::vector<double>& getErrors() const {
			convert();
			return m_errs;
		}


		//constexpr void assign(size_type count, const std::string& value) {
		//	m_isConverted = false;
		//	m_strs.assign(count, value);
		//	return;
		//}

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

		// extereme iterators
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


		//data pointer
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
			return;
		}
		constexpr size_type capacity() const noexcept {
			return m_strs.capacity();
		}
		constexpr void shrink_to_fit() {
			m_strs.shrink_to_fit();
			m_dbls.shrink_to_fit();
			m_errs.shrink_to_fit();
			return;
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
		constexpr void swap(Datavalue& other) {
			m_strs.swap(other.m_strs);
			m_dbls.swap(other.m_dbls);
			m_errs.swap(other.m_errs);
			std::swap(m_isConverted, other.m_isConverted);
			return;
		}



		//"non"-member functions
		friend constexpr bool operator==(const Datavalue& lhs, const Datavalue& rhs) {
			return lhs.m_strs == rhs.m_strs;
		}
		friend constexpr auto operator<=>(const Datavalue& lhs, const Datavalue& rhs) {
			return lhs.m_strs <=> rhs.m_strs;
		}
		friend constexpr void swap(Datavalue& lhs, Datavalue& rhs) noexcept(noexcept(lhs.swap(rhs))) {
			lhs.swap(rhs);
		}
	};


	class Block {
	public:
		using itemorder = std::variant<int, dataname>;
		//using item = typename dict<dataname, Datavalue>::value_type;

	private:
		dict<dataname, Datavalue> m_block{}; // this is the actual data
		dict<int, std::vector<dataname>> m_loops{}; // keeps track of datanames that are looped together
		std::vector<itemorder> m_item_order{}; // keeps the insertion order
		dict<dataname, dataname> m_true_case{}; // keeps the actual case of the tags used.

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

		struct ConstIterator;

	public:
		Block() = default;
		Block(bool ow) : overwrite(ow) {}

		ConstIterator addItem(dataname tag, Datavalue value) noexcept(false) {
			dataname lowerTag{ row::util::toLower(tag) };

			if (!overwrite && contains(lowerTag)) {
				throw tag_already_exists_error(lowerTag);
			}

			if (!contains(lowerTag) && !isInLoop(lowerTag)) {
				m_item_order.push_back(lowerTag);
			}

			if (m_true_case.contains(lowerTag)) {
				m_true_case.erase(lowerTag);
			}
			m_true_case.insert({ lowerTag, tag });
			m_block.insert({ lowerTag, std::move(value) });

			return find(lowerTag);
		}

		ConstIterator addItems(const std::vector<dataname>& tags, const std::vector<Datavalue>& values) noexcept(false) {
			if (tags.size() != values.size()) {
				throw tag_value_mismatch_error(std::format("{} tags and {} values", tags.size(), values.size()));
			}

			for (size_t i = 0; i < tags.size(); ++i) {
				addItem(tags[i], values[i]);
			}
			return find(row::util::toLower(tags[0]));
		}

		ConstIterator addItemsAsLoop(const std::vector<dataname>& tags, const std::vector<Datavalue>& values) noexcept(false) {
			if (tags.size() != values.size()) {
				throw tag_value_mismatch_error(std::format("{} tags and {} lots of values", tags.size(), values.size()));
			}

			size_t len{ values[0].size()};
			if (!(std::all_of(values.cbegin(), values.cend(), [len](const auto& value) { return value.size() == len; }))) {
				throw loop_length_mismatch_error("Different number of values per tag in loop");
			}

			for (size_t i = 0; i < tags.size(); ++i) {
				addItem(tags[i], values[i]);
			}
			return createLoop(tags);
		}

		ConstIterator createLoop(const std::vector<dataname>& tags) noexcept(false) {
			std::vector<dataname> lowerTags{ row::util::toLower(tags) };

			//check that all tags exist, and have all the same length values
			for (const auto& tag : lowerTags) {
				if (!m_block.contains(tag)) {
					throw no_such_tag_error(std::format("{} does not exist.", tag));
				}
			}

			size_t len{ m_block[lowerTags[0]].size() };
			for (const auto& tag : lowerTags) {
				if (m_block.at(tag).size() != len) {
					throw loop_length_mismatch_error("Different number of values per tag in loop");
				}
			}

			//remove all tags from any existing loops
			for (auto& [_, loopTags] : m_loops) {
				for (auto& tag : lowerTags) {
					if (row::util::contains(loopTags, tag)) {
						auto it = std::find(loopTags.cbegin(), loopTags.cend(), tag);
						loopTags.erase(it);
					}
				}
			}

			//remove empty loops
			for (auto& [loopNum, loopTags] : m_loops) {
				if (loopTags.size() == 0) {
					m_loops.erase(loopNum);
				}
			}

			//get the loop number to use for the loop we're about to insert
			int loopNum{ 1 };
			if (m_loops.size() > 0) {
				for (auto& [k, v] : m_loops) {
					if (k >= loopNum)
						loopNum = k + 1;
				}
			}
			m_loops[loopNum] = lowerTags;
			m_item_order.emplace_back(loopNum);

			//remove tags from item order
			for (auto& tag : m_loops[loopNum]) {
				auto it = m_item_order.begin();
				while (it != m_item_order.end()) {
					if (it->index() == 0) {
						++it;
					}
					else if (std::get<std::string>(*it) == tag) {
						it = m_item_order.erase(it);
					}
					else {
						++it;
					}
				}
			}
			return find(lowerTags[0]);
		}

		ConstIterator addNameToLoop(const dataname& newName, const dataname& oldName) noexcept(false) {
			dataname lowerNew{ row::util::toLower(newName) };
			dataname lowerOld{ row::util::toLower(oldName) };

			if (!contains(lowerNew)) { throw no_such_tag_error(std::format("{} does not exist.", lowerNew)); }
			if (m_block.at(lowerNew).size() != m_block.at(lowerOld).size()) { throw loop_length_mismatch_error(std::format("{} new values and {} existing values.", m_block.at(lowerNew).size(), m_block.at(lowerOld).size()));; }

			int loopNum{ getLoopNum(oldName) };
			if (loopNum < 0) { throw no_such_tag_error(std::format("{} does not exist in a loop.", oldName)); }
			if (row::util::contains(m_loops.at(loopNum), lowerNew)) { return find(lowerNew); }

			// if we get to here, the newName exists, the oldName is in a loop,  newName isn't in the oldName loop, and newName length is the same as oldName's

			//remove from other loops
			for (auto& [_, v] : m_loops) {
				std::erase(v, lowerNew);
			}
			m_loops[loopNum].push_back(lowerNew);
			std::erase(m_item_order, itemorder{ lowerNew });

			return find(lowerNew);

		}

		int getLoopNum(const std::string& tag) const {
			dataname lowerTag{ row::util::toLower(tag) };

			for (auto& [k, v] : m_loops) {
				if (row::util::contains(v, lowerTag)) {
					return k;
				}
			}
			return -1;
		}

		const std::vector<dataname>& getLoopNames(const dataname& tag) const noexcept(false) {
			for (auto& [_, vs] : m_loops) {
				if (row::util::contains(vs, tag)) {
					return vs;
				}
			}
			throw no_such_tag_error(std::format("{} does not exist in a loop.", tag));
		}

		bool isInLoop(const dataname& tag) const {
			return getLoopNum(tag) != -1;
		}

		ConstIterator removeItem(const dataname& tag) {
			dataname lowerTag{ row::util::toLower(tag) };

			if (!contains(lowerTag)) {
				return cend();
			}

			ConstIterator r = ++find(lowerTag);

			int loopNum{ getLoopNum(tag) };
			m_block.erase(lowerTag);
			m_true_case.erase(lowerTag);

			if (loopNum > 0) {
				std::erase(m_loops[loopNum], lowerTag);

				if (m_loops[loopNum].size() == 0) {
					m_loops.erase(loopNum);
					std::erase_if(m_item_order, [loopNum](const auto& thing) { if (thing.index() == 0) { return std::get<int>(thing) == loopNum; } else { return false; }});
				}
			}
			else {
				std::erase_if(m_item_order, [lowerTag](const auto& thing) { if (thing.index() == 1) { return std::get<std::string>(thing) == lowerTag; } else { return false; }});
			}
			return r;
		}

		Datavalue getAssociatedValue(const dataname& tag, const std::string& value, const dataname& associatedTag) {
			dataname lowerTag{ row::util::toLower(tag) };
			dataname lowerOther{ row::util::toLower(associatedTag) };
			if (!contains(lowerTag)) {
				throw no_such_tag_error(std::format("{} does not exist.", lowerTag));
			}
			if (!contains(lowerOther)) {
				throw no_such_tag_error(std::format("{} does not exist.", lowerOther));
			}
			if (!isInLoop(lowerTag)) {
				throw no_such_tag_error(std::format("{} does not exist in a loop.", lowerTag));
			}
			if (!isInLoop(lowerOther)) {
				throw no_such_tag_error(std::format("{} does not exist in a loop.", lowerOther));
			}
			if (getLoopNum(lowerTag) != getLoopNum(lowerOther)) {
				throw no_such_tag_error(std::format("{} does not exist in a loop with {}.", lowerOther, lowerTag));
			}
			
			int i{ row::util::getIndexOf(get(tag).getStrings(), value)};

			return Datavalue{ get(associatedTag).getStrings()[i] };

		}


		std::tuple<int, int> getItemPosition(const dataname& tag) const {
			/*A utility function to get the numerical order in the printout
		of `tag`.  An item has coordinate `(loop_no,pos)` with
		the top level having a `loop_no` of -1.
		Return -1, -1, indicates doesn't exist.
		*/
			dataname lowerTag{ row::util::toLower(tag) };

			if (!contains(lowerTag)) {
				return std::tuple<int, int> {-1, -1};
			}
			itemorder tmp{ lowerTag };
			if (row::util::contains(m_item_order, tmp)) {
				return std::tuple<int, int> {-1, row::util::getIndexOf(m_item_order, tmp)};
			}
			else {
				int loopNum = getLoopNum(lowerTag);
				auto it = std::find(m_loops.at(loopNum).cbegin(), m_loops.at(loopNum).cend(), lowerTag);
				int loopPos = static_cast<int>(it - m_loops.at(loopNum).cbegin());
				return std::tuple<int, int> {loopNum, loopPos};
			}
		}

		ConstIterator changeItemPosition(const dataname& tag, const size_t newPosn) {
			/*Move the printout order of `tag` to `newpos`. If `tag` is
				in a loop, `newpos` refers to the order within the loop.*/
			dataname lowerTag{ row::util::toLower(tag) };
			if (!contains(lowerTag)) {
				throw no_such_tag_error(std::format("{} does not exist.", tag));
			}
			auto [loopNum, oldPosn] = getItemPosition(tag);
			if (loopNum < 0) {
				row::util::move_element(m_item_order, oldPosn, newPosn);
			}
			else {
				row::util::move_element(m_loops[loopNum], oldPosn, newPosn);
			}
			return find(lowerTag);
		}

		ConstIterator changeLoopPosition(const dataname& tag, const size_t newPosn) {
			/*Move the printout order of the loop containing `tag` to `newpos`.*/
			dataname lowerTag{ row::util::toLower(tag) };
			int loopNum = getLoopNum(tag);
			if (loopNum < 0) {
				throw no_such_tag_error(std::format("{} does not exist in a loop.", tag));
			}

			itemorder tmp{ loopNum };
			row::util::move_element(m_item_order, row::util::getIndexOf(m_item_order, tmp), newPosn);

			return find(lowerTag);
		}

		//why is this here?
		std::vector<std::string> getInvalidLoopLengths() const {
			std::vector<std::string> r{};
			for (const auto& [k, vs] : m_loops) {
				bool invalidLoopLength{ false };
				size_t loopLen{vs[0].size()};
				for (const auto& v : vs) {
					if (v.size() != loopLen) {
						invalidLoopLength = true;
						break;
					}
				}
				if (invalidLoopLength) {
					r.insert(r.end(), vs.cbegin(), vs.cend());
				}
			}
			return r;
		}

		ConstIterator set(const dataname& tag, const Datavalue& value) {
			return addItem(tag, value);
		}
		ConstIterator put(const dataname& tag, const Datavalue& value) {
			return set(tag, value);
		}
		const Datavalue& get(const dataname& tag) const {
			dataname lowerTag{ row::util::toLower(tag) };
			if (contains(lowerTag)) {
				return m_block.at(lowerTag);
			}
			throw no_such_tag_error(std::format("{} does not exist.", lowerTag));
		}


		std::vector<dataname> getNames() const {
			std::vector<dataname> names{};			
			for (const auto& [k, _] : *this) {
				names.push_back(k);
			}
			return names;
		}
		std::vector<dataname> tags() const {
			return getNames();
		}

		std::vector<Datavalue> getValues() const {
			std::vector<Datavalue> values{};
			for (const auto& [_, v] : *this) {
				values.push_back(v);
			}
			return values;
		}
		std::vector<Datavalue> values() const {
			return getValues();
		}





		std::string makeStringLength(dataname tag, size_t len) const {
			return std::format("{1:{0}}", len, std::move(tag));
		}

		std::string formatValue(std::string value) const {
			if (value.find('\n') != std::string::npos) {
				value = "\n;\n" + value + "\n;";
			}
			else if (value.find(' ') != std::string::npos || value[0] == '_') {
				value = "\"" + value + "\"";
			}
			return value;
		}

		void print(bool pretty = true) const {
			std::cout << to_string(pretty);
		}

		std::string to_string(bool pretty=true) const {
			std::string block{};
			size_t maxTagLen{ 1 }; //must not be zero or std::format throws.

			if (pretty) {
				for (const auto& [k, _] : m_block) {
					maxTagLen = std::max(maxTagLen, k.size());
				}
			}

			for (const auto& item : m_item_order) {
				if (item.index() == 0) { // it's a loop
					int loopNum{ std::get<int>(item) };
					size_t loopLen{ m_block.at(m_loops.at(loopNum)[0]).size() };
					size_t loopWidth{ m_loops.at(loopNum).size()};

					block += "loop_\n";
					for (const auto& tag : m_loops.at(loopNum)) {
						block += "  " + tag + '\n';
					}
					std::vector<size_t> colWidths(m_loops.at(loopNum).size(), 1); //must not be zero or std::format throws.

					if (pretty) {
						for (size_t j{ 0 }; j < loopWidth; ++j) {
							for (size_t i{ 0 }; i < loopLen; ++i) {
								const std::string& value{ m_block.at(m_loops.at(loopNum)[j]).at(i) };

								if (value.find('\n') != std::string::npos) [[unlikely]] {
									continue; //it's a semicolon textfield, and I don't want to mess with it's length.
								}
								colWidths[j] = std::max(colWidths[j], value.size());
							}
						}
					}

					for (size_t i{ 0 }; i < loopLen; ++i) {
						for (size_t j{ 0 }; j < m_loops.at(loopNum).size(); ++j) {
							const std::string& tag{ m_loops.at(loopNum)[j] };
							block += '\t' + makeStringLength(m_block.at(tag).at(i), colWidths[j]);
						}
						block += '\n';
					}
				}
				else { // it's a plain dataitem
					const std::string& loopTag = std::get<std::string>(item);
					block += makeStringLength(loopTag, maxTagLen) + '\t' + formatValue(m_block.at(loopTag).at(0)) + '\n';
				}
			}
			return block;
		}



		//// taken from https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
		//struct Iterator
		//{
		//	using iterator_category = std::forward_iterator_tag;
		//	using difference_type = std::ptrdiff_t;
		//	using value_type = std::pair<const dataname, datavalue>;
		//	using pointer = value_type*;
		//	using reference = value_type&;
		//
		//	Iterator(pointer m_ptr, Block* blk)
		//		: m_ptr{ m_ptr }, block{ blk } { }
		//	
		//	reference operator*() const { return *m_ptr; }
		//	pointer operator->() { return m_ptr; }
		//
		//	// Prefix increment
		//	Iterator& operator++() { 
		//		dataname currentTag{ m_ptr->first };
		//		auto [loopNum, posn] = block->getItemPosition(currentTag);
		//		itemorder nextItem{};
		//		if (loopNum < 0) { // currentTag is not a loop
		//			if (posn < block->m_item_order.size() - 1) { // not at the end of the item_order
		//				nextItem = block->m_item_order[++posn];
		//				if (nextItem.index() == 0) { // the nextkey is the start of a loop
		//					currentTag = block->m_loops.at(std::get<int>(nextItem))[0];//update the keyname
		//				}
		//				else { //the next key is just a key
		//					currentTag = std::get<dataname>(nextItem); //update the keyname
		//				}
		//				m_ptr = &(*block->m_block.find(currentTag));
		//			}
		//			else {
		//				m_ptr = nullptr;
		//			}
		//		}
		//		else {  //we're in a loop
		//			if (posn < block->m_loops[loopNum].size() - 1) { // not at the end of the loop
		//				currentTag = block->m_loops[loopNum][++posn];
		//				m_ptr = &(*block->m_block.find(currentTag));
		//			}
		//			else { //we're at the end of a loop and need to look at the next element
		//				int currentIndex = row::util::getIndexOf(block->m_item_order, std::variant<int, dataname>{loopNum});
		//				if (currentIndex < block->m_item_order.size() - 1) { // we can go to the next element
		//					nextItem = block->m_item_order[++currentIndex];
		//					if (nextItem.index() == 0) { // the nextkey is the start of a loop
		//						currentTag = block->m_loops.at(std::get<int>(nextItem))[0];//update the keyname
		//					}
		//					else { //the next key is just a key
		//						currentTag = std::get<dataname>(nextItem); //update the keyname
		//					}
		//					m_ptr = &(*block->m_block.find(currentTag));
		//				}
		//				else {
		//					m_ptr = nullptr;
		//				}
		//			}
		//		}
		//		return *this; 
		//	}
		//
		//	// Postfix increment
		//	Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		//
		//	friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		//	friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
		//
		//private:
		//	pointer m_ptr;
		//	Block* block;
		//};


		//// taken from https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
		struct ConstIterator
		{
		public:		
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = std::pair<const dataname, Datavalue>;
			using pointer = value_type*;
			using reference = value_type&;
			using const_pointer = const std::pair<const dataname, Datavalue>*;
			using const_reference = const std::pair<const dataname, Datavalue>&;

		private:
			const_pointer m_ptr;
			const Block* block;
		
		public:
			ConstIterator(const_pointer m_ptr, const Block* blk)
				: m_ptr{ m_ptr }, block{ blk } { }

			const_reference operator*() const { return *m_ptr; }
			const_pointer operator->() { return m_ptr; }

			// Prefix increment
			ConstIterator& operator++() {
				dataname currentTag{  };

				//what is ++end()? It should be begin();
				if (!m_ptr) { //ie, increment end();
					size_t numItems{ block->m_item_order.size() };
					itemorder lastItem{ block->m_item_order[numItems - 1] };
					if (lastItem.index() == 0) { // the lastItem is the start of a loop
						size_t numTags{ block->m_loops.at(std::get<int>(lastItem)).size() };
						currentTag = block->m_loops.at(std::get<int>(lastItem))[numTags - 1];//update the tagname
					}
					else { //the next tag is just a tag
						currentTag = std::get<dataname>(lastItem); //update the tagname
					}
					m_ptr = &(*block->m_block.find(currentTag));
					return *this;
				}


				currentTag = m_ptr->first;
				auto [loopNum, posn] = block->getItemPosition(currentTag);
				itemorder nextItem{};
				if (loopNum < 0) { // currentTag is not a loop
					if (posn < block->m_item_order.size() - 1) { // not at the end of the item_order
						m_ptr = nextPtr(posn, currentTag);
					}
					else {
						m_ptr = nullptr;
					}
				}
				else {  //we're in a loop
					if (posn < block->m_loops.at(loopNum).size() - 1) { // not at the end of the loop
						currentTag = block->m_loops.at(loopNum)[++posn];
						m_ptr = const_cast<pointer>(&(*block->m_block.find(currentTag)));
					}
					else { //we're at the end of a loop and need to look at the next element
						int currentIndex = row::util::getIndexOf(block->m_item_order, std::variant<int, dataname>{ loopNum });
						if (currentIndex < block->m_item_order.size() - 1) { // we can go to the next element
							m_ptr = nextPtr(currentIndex, currentTag);
						}
						else {
							m_ptr = nullptr;
						}
					}
				}
				return *this;
			}

			// Prefix decrement
			ConstIterator& operator--() {

				dataname currentTag{};
				// what is --end()? It should be begin();
				if (!m_ptr) { //ie, decrement end();
					itemorder firstItem{ block->m_item_order[0] };
					if (firstItem.index() == 0) { // the firstTag is the start of a loop
						currentTag = block->m_loops.at(std::get<int>(firstItem))[0];//update the tagname
					}
					else { //the next tag is just a tag
						currentTag = std::get<dataname>(firstItem); //update the tagname
					}
					m_ptr = &(*block->m_block.find(currentTag));
					return *this;
				}

				currentTag = m_ptr->first;
				auto [loopNum, posn] = block->getItemPosition(currentTag);
				if (loopNum < 0) { // currentTag is not a loop
					if (posn > 0) { // not at the beginning of the item_order
						m_ptr = prevPtr(posn, currentTag);
					}
					else {
						m_ptr = nullptr;
					}
				}
				else {  //we're in a loop
					if (posn > 0) { // not at the beginning of the loop
						currentTag = block->m_loops.at(loopNum)[--posn];
						m_ptr = &(*block->m_block.find(currentTag));
					}
					else { //we're at the beginning of a loop and need to look at the next element
						int currentIndex{ row::util::getIndexOf(block->m_item_order, std::variant<int, dataname>{ loopNum }) };
						if (currentIndex < 0) { // we can go to the previous element
							m_ptr = prevPtr(currentIndex, currentTag);
						}
						else {
							m_ptr = nullptr;
						}
					}
				}
				return *this;
			}

			// Postfix increment/decrement
			ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }
			ConstIterator operator--(int) { ConstIterator tmp = *this; --(*this); return tmp; }

			friend bool operator== (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr == b.m_ptr; };
			friend bool operator!= (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr != b.m_ptr; };

		private:
			const_pointer getPtr(size_t idx, dataname& currentTag) {
				itemorder item{ block->m_item_order[idx] };
				if (item.index() == 0) { // the prevTag is the start of a loop
					size_t numTags{ block->m_loops.at(std::get<int>(item)).size() };
					currentTag = block->m_loops.at(std::get<int>(item))[numTags - 1];//update the tagname
				}
				else { //the next tag is just a tag
					currentTag = std::get<dataname>(item); //update the tagname
				}
				return &(*block->m_block.find(currentTag));
			}

			const_pointer prevPtr(size_t currIndex, dataname& currentTag) {
				return getPtr(--currIndex, currentTag);
			}

			const_pointer nextPtr(size_t currIndex, dataname& currentTag) {
				return getPtr(++currIndex, currentTag);
			}





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
		int size(const dataname& tag) const noexcept {
			if (contains(tag)) {
				return static_cast<int>(m_block.at(tag).size());
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
		size_t erase(const dataname& tag) {
			ConstIterator r = removeItem(tag);
			if (r == this->cend())
				return 0;
			else
				return 1;
		}

		// Lookup
		const Datavalue& at(const dataname& tag) const {
			dataname lowerTag{ row::util::toLower(tag) };
			return m_block.at(lowerTag);
		}
		size_t count(const dataname& tag) const {
			dataname lowerTag{ row::util::toLower(tag) };
			return m_block.count(lowerTag);
		}
		ConstIterator find(const dataname& tag) const {
			dataname lowerTag{ row::util::toLower(tag) };

			auto it = m_block.find(lowerTag);
			if (it == m_block.end()) {
				return cend();
			}

			std::pair<const dataname, Datavalue>* m_ptr = const_cast<std::pair<const dataname, Datavalue>*>(&(*it));
			return ConstIterator(m_ptr, this);
		}
		bool contains(const dataname& tag) const {
			dataname lowerTag{ row::util::toLower(tag) };
			return m_block.contains(lowerTag);
		}



	private:
		std::pair<const dataname, Datavalue>* ptrToFirstItem() {
			itemorder firstItem = m_item_order[0];
			dataname firstTag{};
			if (firstItem.index() == 0) {
				int loopNum = std::get<int>(firstItem);
				firstTag = m_loops.at(loopNum)[0];
			}
			else {
				firstTag = std::get<dataname>(firstItem);
			}
			return &(*m_block.find(firstTag));
		}

		std::pair<const dataname, Datavalue>* constptrToFirstItem() const {
			itemorder firstItem = m_item_order[0];
			dataname firstTag{};
			if (firstItem.index() == 0) {
				int loopNum = std::get<int>(firstItem);
				firstTag = m_loops.at(loopNum)[0];
			}
			else {
				firstTag = std::get<dataname>(firstItem);
			}
			std::pair<const dataname, Datavalue>* ptr = const_cast<std::pair<const dataname, Datavalue>*>(&(*m_block.find(firstTag)));
			return ptr;
		}
	};


	class Cif {
	public:
		using blockname = std::string;

	private:
		dict<blockname, Block> m_cif{}; // this is the actual data
		std::vector<blockname> m_block_order{}; // keeps the insertion order
		dict<blockname, blockname> m_true_case{}; // keeps the actual case of the blocknames used.

		std::string m_source{};
		bool m_overwrite{ true };
	public:

		//these print_* functions are for debugging purposes
		void print_cif() const {
			std::cout << "---\nCIF tags:\n";
			for (const auto& [k, _] : m_cif) {
				std::cout << k << '\n';
			}
			std::cout << "---\n";
		}

		void print_block_order() const {
			std::cout << "---\nBlock order:\n";
			for (const auto& block : m_block_order) {
				std::cout << block << '\n';
			}
			std::cout << "---\n";
		}

		void print_true_case() const {
			std::cout << "---\nTrue case:\n";
			for (const auto& [k, v] : m_true_case) {
				std::cout << k << " : " << v << ", ";
			}
			std::cout << "\n---\n";
		}

		struct ConstIterator;
	public:
		Cif() {}
		explicit Cif(std::string source) : m_source(std::move(source)) {}

		Block& getLastBlock() {
			return m_cif.at(m_block_order.back());
		}
		const Block& getLastBlock() const {
			return m_cif.at(m_block_order.back());
		}

		const std::string& getSource() const {
			return m_source;
		}


		Block& addName(const blockname& name) noexcept(false) {
			dataname lowerTag{ row::util::toLower(name) };

			if (!canOverwrite() && contains(lowerTag)) {
				throw tag_already_exists_error(lowerTag);
			}

			if (!contains(lowerTag)) {
				m_block_order.push_back(lowerTag);
			}

			m_true_case.erase(lowerTag);
			m_true_case[lowerTag] = name;
			m_cif[lowerTag];
			m_cif[lowerTag].overwrite = m_overwrite;
			return m_cif[lowerTag];
		}

		ConstIterator addBlock(const blockname& name, const Block& block) noexcept(false) {
			dataname lowerTag{ row::util::toLower(name) };

			if (!canOverwrite() && contains(lowerTag)) {
				throw tag_already_exists_error(lowerTag);
			}

			if (!contains(lowerTag)) {
				m_block_order.push_back(lowerTag);
			}

			m_true_case.erase(lowerTag);
			m_true_case[lowerTag] = name;
			m_cif[lowerTag] = block;
			return find(lowerTag);
		}

		ConstIterator addBlocks(const std::vector<blockname>& names, const std::vector<Block>& blocks) {
			if (names.size() != blocks.size()) {
				throw tag_value_mismatch_error(std::format("{} names and {} blocks", names.size(), blocks.size()));
			}

			for (size_t i = 0; i < names.size(); ++i) {
				addBlock(names[i], blocks[i]);
			}
			return find(row::util::toLower(names[0]));
		}

		ConstIterator removeBlock(const blockname& name) {
			dataname lowerTag{ row::util::toLower(name) };

			if (!contains(lowerTag)) {
				return cend();
			}
			ConstIterator r = ++find(lowerTag);
			m_cif.erase(lowerTag);
			m_true_case.erase(lowerTag);
			std::erase_if(m_block_order, [lowerTag](const auto& thing) { return thing == lowerTag; });

			return r;
		}

		int getBlockPosition(const dataname& name) const {
			/*A utility function to get the numerical order in the printout
		of `name`.  An item has coordinate `(loop_no,pos)` with
		the top level having a `loop_no` of -1.
		Return -1, -1, indicates doesn't exist.
		*/
			dataname lowerTag{ row::util::toLower(name) };

			if (!contains(lowerTag)) {
				return -1;
			}
			return row::util::getIndexOf(m_block_order, lowerTag);
		}

		ConstIterator changeBlockPosition(const dataname& name, const size_t newPosn) {
			/*Move the printout order of `name` to `newpos`. If `name` is
				in a loop, `newpos` refers to the order within the loop.*/
			dataname lowerTag{ row::util::toLower(name) };
			if (!contains(lowerTag)) {
				throw no_such_tag_error(std::format("{} does not exist.", lowerTag));
			}

			int oldPosn = getBlockPosition(name);
			row::util::move_element(m_block_order, oldPosn, newPosn);

			return find(lowerTag);
		}

		bool overwrite(bool ow) {
			if (m_overwrite == ow) {
				return m_overwrite == ow;
			}
			m_overwrite = ow;

			for (auto& [_, block] : m_cif) {
				block.overwrite = m_overwrite;
			}
			return m_overwrite;
		}

		bool canOverwrite() const {
			return m_overwrite;
		}

		void print(bool pretty = true) const {
			std::cout << to_string(pretty);
		}

		std::string to_string(bool pretty=true) const {
			std::string cif{};
			for (const auto& block : m_block_order) {
				cif += "\ndata_" + block + '\n';
				cif += m_cif.at(block).to_string(pretty);
			}
			return cif;
		}



		//struct Iterator
		//// taken from https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
		//{
		//	using iterator_category = std::forward_iterator_tag;
		//	using difference_type = std::ptrdiff_t;
		//	using value_type = std::pair<const blockname, Block>;
		//	using pointer = value_type*;
		//	using reference = value_type&;
		//
		//	Iterator(pointer m_ptr, Cif* cif)
		//		: m_ptr{ m_ptr }, cif{ cif } { }
		//
		//	reference operator*() const { return *m_ptr; }
		//	pointer operator->() { return m_ptr; }
		//
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
		//
		//	// Postfix increment
		//	Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		//
		//	friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		//	friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
		//
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
		int size(const std::string& name) const noexcept {
			dataname lowerTag{ row::util::toLower(name) };
			if (contains(lowerTag)) {
				return static_cast<int>(m_cif.at(lowerTag).size());
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
			m_overwrite = true;
		}
		size_t erase(const blockname& name) {
			dataname lowerTag{ row::util::toLower(name) };
			ConstIterator r{ removeBlock(lowerTag) };
			if (r != cend()) { return 1; }
			else { return 0; }
		}
		ConstIterator set(const blockname& name, const Block& value) {
			return addBlock(name, value);
		}
		ConstIterator put(const blockname& name, const Block& value) {
			return set(name, value);
		}


		// Lookup
		const Block& get(const blockname& name) const {
			dataname lowerTag{ row::util::toLower(name) };
			if (contains(name)) {
				return m_cif.at(name);
			}
			throw no_such_tag_error(std::format("{} does not exist.", lowerTag));
		}
		const Block& at(const blockname& name) const {
			dataname lowerTag{ row::util::toLower(name) };
			return m_cif.at(lowerTag);
		}
		size_t count(const blockname& name) const {
			dataname lowerTag{ row::util::toLower(name) };
			return m_cif.count(lowerTag);
		}
		ConstIterator find(const blockname& name) const {
			dataname lowerTag{ row::util::toLower(name) };
			std::pair<const blockname, Block>* m_ptr = const_cast<std::pair<const blockname, Block>*>(&(*m_cif.find(lowerTag)));
			return ConstIterator(m_ptr, this);
		}
		bool contains(const blockname& name) const {
			dataname lowerTag{ row::util::toLower(name) };
			return m_cif.contains(lowerTag);
		}


	private:
		std::pair<const blockname, Block>* ptrToFirstBlock() {
			return &(*m_cif.find(m_block_order[0]));
		}

		std::pair<const blockname, Block>* constptrToFirstBlock() const {
			std::pair<const blockname, Block>* ptr = const_cast<std::pair<const blockname, Block>*>(&(*m_cif.find(m_block_order[0])));
			return ptr;
		}
	};
}

