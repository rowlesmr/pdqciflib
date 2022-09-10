
#ifndef ROW_CIFPARSE_HPP
#define ROW_CIFPARSE_HPP


#include <iostream>
#include <stdexcept>
#include "tao/pegtl.hpp"

#include "ciffile.hpp"
#include "cifexcept.hpp"

namespace row::cif {

    namespace pegtl = tao::pegtl;

    namespace rules {
        //reserved words
        struct DATA : TAO_PEGTL_ISTRING("data_") {};
        struct LOOP : TAO_PEGTL_ISTRING("loop_") {};
        struct GLOBAL : TAO_PEGTL_ISTRING("global_") {};
        struct SAVE : TAO_PEGTL_ISTRING("save_") {};
        struct STOP : TAO_PEGTL_ISTRING("stop_") {};
        struct reserved : pegtl::sor<DATA, LOOP, SAVE, GLOBAL, STOP> {};

        //character sets
        //OrdinaryChar:	{ '!' | '%' | '&' | '(' | ')' | '*' | '+' | ',' | '-' | '.' | '/' | '0'
        //              | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | ':' | '<' | '='
        //              | '>' | '?' | '@' | 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I'
        //              | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U'
        //              | 'V' | 'W' | 'X' | 'Y' | 'Z' | '\' | '^' | '`' | 'a' | 'b' | 'c' | 'd'
        //              | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p'
        //              | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z' | '{' | '|'
        //              | '}' | '~' }
        //NonBlankChar:	OrdinaryChar | dquote | '#' | '$' | squote | '_' |           ';' | '[' | ']'
        //TextLeadChar:	OrdinaryChar | dquote | '#' | '$' | squote | '_' | SP | HT |       '[' | ']'
        //AnyPrintChar:	OrdinaryChar | dquote | '#' | '$' | squote | '_' | SP | HT | ';' | '[' | ']'
        struct ordinarychar : pegtl::ranges<'!', '!', '%', '&', '(', ':', '<', 'Z', '\\', '\\', '^', '^', '`', '~'> {}; //not: ' " # $ _ ; [ ] SP HT
        struct nonblankchar : pegtl::range<'!', '~'> {};  //not: SP HT
        struct textleadchar : pegtl::ranges<' ', ':', '<', '~', '\t'> {}; //all chars except ';'
        struct anyprintchar : pegtl::ranges<' ', '~', '\t'> {}; //ALL THE CHARS!!

        //Whitespace and comments
        struct comment : pegtl::if_must<pegtl::one<'#'>, pegtl::until<pegtl::eolf>> {};
        struct ws : pegtl::blank {}; // pegtl::one<' ', '\t'>
        struct wschar : pegtl::space {}; //pegtl::one<' ', '\n', '\r', '\t', '\v', '\f'>
        struct whitespace : pegtl::plus<pegtl::sor<wschar, comment>> {};
        struct ws_or_eof : pegtl::sor<whitespace, pegtl::eof> {};

        //character text fields
        struct field_sep : pegtl::seq<pegtl::bol, pegtl::one<';'>> {};
        struct end_field_sep : pegtl::seq<pegtl::star<ws>, pegtl::plus<pegtl::eol>, field_sep> {};
        struct sctf_text : pegtl::star<pegtl::not_at<end_field_sep>, pegtl::sor<anyprintchar, pegtl::eol>> {};
		struct semicolontextfield : pegtl::if_must<field_sep, sctf_text, end_field_sep> {};

        //rules for folded semicolon textfields
        struct fsctf_start : pegtl::seq<field_sep, pegtl::one<'\\'>, pegtl::eol> {};
        struct fsctf_text_for_parsing : sctf_text {};
        struct foldedsemicolontextfield : pegtl::if_must<fsctf_start, fsctf_text_for_parsing, end_field_sep> {};

        struct fsctf_continuation : pegtl::seq < pegtl::one<'\\'>, pegtl::star<pegtl::blank>, pegtl::eol> {};
        struct fsctf_text : pegtl::plus<pegtl::not_at<fsctf_continuation>, pegtl::sor<anyprintchar, pegtl::eol>> {};
        struct fsctf_line_without_continuation : fsctf_text {};
        struct fsctf_line_with_continuation : pegtl::seq<fsctf_text, fsctf_continuation> {};
        struct fsctf_grammar : pegtl::seq<pegtl::star<pegtl::plus<fsctf_line_with_continuation>, pegtl::star<fsctf_line_without_continuation>>, fsctf_line_without_continuation> {};

        struct textfield : pegtl::sor<foldedsemicolontextfield, semicolontextfield> {};


        //quoted and unquoted strings
        template<typename Q> struct endq : pegtl::seq<Q, pegtl::at<pegtl::sor<pegtl::one<' ', '\n', '\r', '\t', '#'>, pegtl::eof>>> {}; //what is the end of a quoted string
        template<typename Q> struct quote_text : pegtl::seq<pegtl::star<pegtl::not_at<endq<Q>>, anyprintchar>> {};
        struct unquoted_text : pegtl::plus<nonblankchar> {};
        template <typename Q> struct quoted_tail : pegtl::seq<quote_text<Q>, endq<Q>> {}; //the entire tail of a quoted string
        template<typename Q> struct quoted : pegtl::if_must<Q, quoted_tail<Q>> {};

        struct singlequotedstring : quoted<pegtl::one<'\''>> {};
        struct doublequotedstring : quoted<pegtl::one<'"'>> {};
        struct unquotedstring : pegtl::seq<pegtl::not_at<reserved>, pegtl::not_at<pegtl::one<'_', '$', '#', '['>>, pegtl::plus<nonblankchar>> {};
        struct charstring : pegtl::sor<singlequotedstring, doublequotedstring, unquotedstring> {};

        // for pdCIF, most values will be numeric. But we don't care about their specific type, we just suck up their
        //  string representation, and worry about it later.
        struct numeric : pegtl::seq<pegtl::plus<ordinarychar>, pegtl::at<wschar>> {};

        //Tags and values
        struct tag : pegtl::seq<pegtl::one<'_'>, pegtl::sor<pegtl::plus<nonblankchar>, TAO_PEGTL_RAISE_MESSAGE("Malformed tag name.")>> {};
        struct value : pegtl::sor<numeric, textfield, charstring> {}; //need to check for textfield first, as a charstring can start with a semicolon
        struct itemtag : tag {};
        struct itemvalue : value {};
        struct looptag : tag {};
        struct loopvalue : value {};

        //CIF Structure
        //loop
        struct loopend : pegtl::opt<STOP, ws_or_eof> {};
        struct looptags : pegtl::plus<pegtl::seq<looptag, whitespace, pegtl::discard>> {};
        struct loopvalues : pegtl::sor<pegtl::plus<pegtl::seq<loopvalue, ws_or_eof, pegtl::discard>>, /* handle incorrect CIF with empty loop -->*/ pegtl::at<pegtl::sor<reserved, pegtl::eof>>> {};
        struct loopstart : pegtl::seq<LOOP, whitespace> {};
        struct loop : pegtl::if_must<loopstart, looptags, loopvalues, loopend> {};

        //pair
        struct pair : pegtl::if_must<itemtag, whitespace, pegtl::if_then_else<itemvalue, ws_or_eof, TAO_PEGTL_RAISE_MESSAGE("Malformed or missing value.")>, pegtl::discard> {};

        //item
        struct dataitem : pegtl::sor<pair, loop> {};

        // block name
        // data_NAME -> the name is "NAME". The match below shoule be pegtl::plus<nonblankchar>, but there exist otherwise valid
        // CIF files where the name is empty, so I've allowed the name to be empty. I've left the error message in there, as it
        // isn't doing any harm (I think)
        struct blockframecode : pegtl::sor<pegtl::star<nonblankchar>, TAO_PEGTL_RAISE_MESSAGE("Malformed block name.")> {};

        //saveframe
        struct saveframeend : SAVE {};
        struct saveframeheading : pegtl::seq<SAVE, blockframecode> {};
        struct saveframe : pegtl::if_must<saveframeheading, whitespace, pegtl::star<dataitem>, saveframeend, ws_or_eof> {};

        //datablock
        struct datablockheading : pegtl::seq<DATA, blockframecode> {};
        struct datablock : pegtl::seq<datablockheading, ws_or_eof, pegtl::star<pegtl::sor<dataitem, saveframe>>> {};

        //The actual CIF file
        struct content : pegtl::plus<pegtl::seq<datablock, pegtl::opt<whitespace>>> {};
        struct file : pegtl::seq<pegtl::opt<comment>, pegtl::opt<whitespace>, pegtl::if_must<pegtl::not_at<pegtl::eof>, content, pegtl::eof>> {};

    } //end namespace rules


    //to keep track of whether the quote string has been sent to file
    struct Status {
        bool is_loop{ false };
        bool is_quote{ false };
        bool is_printed{ false };
        bool is_fsctf{ false };

		void reset() {
			is_loop = false;
			is_quote = false;
			is_printed = false;
            is_fsctf = false;
		}

		void get_ready_to_print() {
			is_quote = true;
			is_printed = false;
		}

		void just_printed() {
			is_printed = true;
            is_fsctf = false;
		}

		void finished_printing() {
			is_quote = false;
			is_printed = false;
		}

		void loop() {
			is_loop = !is_loop;
		}

    };


    //to temporarily store data before putting it in the Cif object
    struct Buffer {
        dataname tag{};
        
        std::string fsctf_string{};
        std::vector<dataname> tags{};
        std::vector<Datavalue> values{};
        size_t loopNum{};
        size_t maxLoop{};
        size_t totalValues{};
        size_t tagNum{};

		void initialiseValues() {
			if (values.empty()) {
				maxLoop = tags.size();
				values = std::vector<Datavalue>(maxLoop);
			}
		}

		void appendTag(std::string in_tag) {
			tags.push_back(std::move(in_tag));
			++tagNum;
		}

		void appendValue(std::string val) {
			values[loopNum].push_back(std::move(val));
			loopNum = ++loopNum % maxLoop;
			++totalValues;
		}

		void clear() {
            tag.clear();
            fsctf_string.clear();
            tags.clear();
            values.clear();
            loopNum = 0;
            maxLoop = 0;
            totalValues = 0;
            tagNum = 0;
		}
    };


    template<typename Input>
    void divert_action_to_value(const Input& in, Cif& out, Status& status, Buffer& buffer) {
        status.get_ready_to_print();
        if (status.is_loop) {
            Action<rules::loopvalue>::apply(in, out, status, buffer);
        }
        else {
            Action<rules::itemvalue>::apply(in, out, status, buffer);
        }
    }


    //********************
    // Parsing Actions to populate the values in the ciffile
    //********************
    template<typename Rule>
    struct Action : pegtl::nothing<Rule> {};

	template<typename Rule>
	struct Action_inner : pegtl::nothing<Rule> {};

    template<> struct Action<rules::blockframecode> {
        template<typename Input> static void apply(const Input& in, Cif& out, Status& status, [[maybe_unused]] Buffer& buffer) {
            try {
                out.addName(in.string());
            }
            catch (tag_already_exists_error&) {
                throw pegtl::parse_error(std::format("Duplicate blockname found: {0}", in.string()), in);
            }
            status.reset();
        }
    };

    template<> struct Action<rules::saveframeheading> {
        template<typename Input> static void apply(const Input& in, [[maybe_unused]] Cif&, [[maybe_unused]] Status&, [[maybe_unused]] Buffer&) {
            throw pegtl::parse_error("Saveframes are not supported by this parser.", in);
        }
    };

    template<> struct Action<rules::itemtag> {
        template<typename Input> static void apply(const Input& in, [[maybe_unused]] Cif& out, [[maybe_unused]] Status& status, Buffer& buffer) {
            buffer.clear();
            buffer.tag = in.string();
            Block& block = out.getLastBlock();
			try {
				block.addName(buffer.tag);
			}
			catch (tag_already_exists_error&) {
				throw pegtl::parse_error(std::format("Duplicate tag found: {0}", in.string()), in);
			}
        }
    };

    template<> struct Action<rules::itemvalue> {
        template<typename Input> static void apply(const Input& in, Cif& out, Status& status, Buffer& buffer) {
            if (!status.is_quote || (status.is_quote && !status.is_printed)) [[likely]] {
                Block& block = out.getLastBlock();
                try {
                    if(status.is_fsctf){
                        block.assignValue(buffer.tag, buffer.fsctf_string);
                    }
                    else {
                        block.assignValue(buffer.tag, in.string());
                    }
                }
                catch (no_such_tag_error&) {
                    throw pegtl::parse_error(std::format("Tag not found: {0}", in.string()), in);
                }
                status.just_printed();
            }
            else {
                status.finished_printing();
            }
        }
    };

    template<> struct Action<rules::loopstart> {
        template<typename Input> static void apply([[maybe_unused]] const Input& in, [[maybe_unused]] Cif& out, Status& status, Buffer& buffer) {
            buffer.clear();
            status.loop();
        }
    };

    template<> struct Action<rules::looptag> {
        template<typename Input> static void apply(const Input& in, [[maybe_unused]] Cif& out, [[maybe_unused]] Status& status, Buffer& buffer) {
            buffer.appendTag(in.string());
        }
    };

    template<> struct Action<rules::loopvalue> {
        template<typename Input> static void apply(const Input& in, [[maybe_unused]] Cif& out, Status& status, Buffer& buffer) {
            buffer.initialiseValues();
            if (!status.is_quote || (status.is_quote && !status.is_printed)) [[likely]] {
				if (status.is_fsctf) {
					buffer.appendValue(buffer.fsctf_string);
				}
				else {
					buffer.appendValue(in.string());
				}
                status.just_printed();
            }
            else {
                status.finished_printing();
            }
        }
    };

    template<> struct Action<rules::loop> { //this is the end of a loop
        template<typename Input> static void apply(const Input& in, Cif& out, Status& status, Buffer& buffer) {
            if(buffer.values.empty()) [[unlikely]] {
                throw pegtl::parse_error("No values given in loop.", in);
            }

            Block& block = out.getLastBlock();
            try {
                block.addItemsAsLoop(buffer.tags, buffer.values);
            }
            catch (const tag_already_exists_error& e) {
                throw pegtl::parse_error(std::format("Tag \"{0}\" in loop already exists", std::string(e.what())), in);
            }
            catch (const loop_length_mismatch_error&) {
                size_t should_be_zero = buffer.totalValues % buffer.tagNum;
                std::string too_many{ std::to_string(should_be_zero) };
                std::string too_few{ std::to_string(buffer.tagNum - should_be_zero) };
                throw pegtl::parse_error(std::format("{0} too few, or {1} too many, values in loop.", too_few, too_many), in);
            }
            status.loop();
        }
    };

    template<> struct Action<rules::quote_text<pegtl::one<'\''>>> {
        template<typename Input> static void apply(const Input& in, Cif& out, Status& status, Buffer& buffer) {
            divert_action_to_value(in, out, status, buffer);
        }
    };

    template<> struct Action<rules::quote_text<pegtl::one<'\"'>>> {
        template<typename Input> static void apply(const Input& in, Cif& out, Status& status, Buffer& buffer) {
            divert_action_to_value(in, out, status, buffer);
        }
    };

    template<> struct Action<rules::sctf_text> {
        template<typename Input> static void apply(const Input& in, Cif& out, Status& status, Buffer& buffer) {
            divert_action_to_value(in, out, status, buffer);
        }
    };

	template<> struct Action_inner<rules::fsctf_text> {
		template<typename Input> static void apply(const Input& in, std::string& out) {
			out += in.string();
 		}
	};


	template<> struct Action<rules::fsctf_text_for_parsing> {
        template<typename Input> static void apply(const Input& in, Cif& out, Status& status, Buffer& buffer) {
            if(!status.is_fsctf){
                buffer.fsctf_string.clear();
            }
            status.is_fsctf = true;

            pegtl::string_input i2(in.string(), "from fsctf");
            try {
                tao::pegtl::parse_nested< rules::fsctf_grammar, Action_inner >(in, i2, buffer.fsctf_string);
            }
			catch (pegtl::parse_error& e) {
				const auto p = e.positions().front();
				//pretty-print the error msg and the line that caused it, with an indicator at the token that done it.
				std::cerr << e.what() << '\n'
					<< i2.line_at(p) << '\n'
					<< std::setw(p.column) << '^' << std::endl;
				throw std::runtime_error("Parsing error -> " + std::string(e.what()));
			}
            divert_action_to_value(in, out, status, buffer);
        }
	};

    template<typename Input>
    void parse_input(Cif& d, Input&& in, bool printErr = true) noexcept(false) {
        try {
            Status status{};
            Buffer buffer{};
            pegtl::parse<rules::file, Action>(in, d, status, buffer);
        }
        catch (pegtl::parse_error& e) {
            const auto p = e.positions().front();
            //pretty-print the error msg and the line that caused it, with an indicator at the token that done it.
            if (printErr) {
                std::cerr << e.what() << '\n'
                    << in.line_at(p) << '\n'
                    << std::setw(p.column) << '^' << std::endl;
            }
            throw std::runtime_error("Parsing error -> " + std::string(e.what()));
        }
    }

    template<typename Input>
    Cif read_input(Input&& in, bool overwrite = false, bool printErr = true)  noexcept(false) {
        Cif cif{ in.source() };
        cif.overwrite(overwrite);
        parse_input(cif, in, printErr);
        return cif;
    }

    //read in a file into a Cif. Will throw std::runtime_error if it encounters problems
    inline Cif read_file(const std::string& filename, bool overwrite = false, bool printErr = true) noexcept(false) {
		pegtl::file_input in(filename);
		return read_input(in, overwrite, printErr);
	}

    //read a string into a Cif. Will throw std::runtime_error if it encounters problems
    inline Cif read_string(const std::string& cifstring, bool overwrite = false, bool printErr = true, const std::string& source = "string") noexcept(false) {
		pegtl::string_input in(cifstring, source);
		return read_input(in, overwrite, printErr);
	}

}
#endif // !ROW_CIFPARSE_HPP
