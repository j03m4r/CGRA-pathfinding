// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

// "%code top" blocks.
#line 32 "dot.y"

    #include "../DotParserDriver.h"
    #include <sstream>

#line 44 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"




#include "dot.tab.hh"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 143 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"

  /// Build a parser object.
  parser::parser (DotParserDriver& d_yyarg, DOTScannerWrapper& scanner_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      d (d_yyarg),
      scanner (scanner_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_kvp: // kvp
        value.YY_MOVE_OR_COPY< AttributeKVP > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_list: // attr_list
      case symbol_kind::S_maybe_attr_list: // maybe_attr_list
      case symbol_kind::S_a_list: // a_list
        value.YY_MOVE_OR_COPY< AttributeStorage > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_edge_rhs: // edge_rhs
        value.YY_MOVE_OR_COPY< std::list<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_QUOTED_ID: // QUOTED_ID
      case symbol_kind::S_BAD_ID: // BAD_ID
      case symbol_kind::S_HTML_BEGIN: // "<"
      case symbol_kind::S_HTML_END: // ">"
      case symbol_kind::S_DIR_EDGEOP: // DIR_EDGEOP
      case symbol_kind::S_UNDIR_EDGEOP: // UNDIR_EDGEOP
      case symbol_kind::S_HTML_CONTENT: // HTML_CONTENT
      case symbol_kind::S_LEX_ERROR: // LEX_ERROR
      case symbol_kind::S_id: // id
      case symbol_kind::S_html_id: // html_id
      case symbol_kind::S_html: // html
      case symbol_kind::S_edge_target: // edge_target
      case symbol_kind::S_node_id: // node_id
      case symbol_kind::S_subgraph: // subgraph
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_kvp: // kvp
        value.move< AttributeKVP > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_attr_list: // attr_list
      case symbol_kind::S_maybe_attr_list: // maybe_attr_list
      case symbol_kind::S_a_list: // a_list
        value.move< AttributeStorage > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_edge_rhs: // edge_rhs
        value.move< std::list<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_QUOTED_ID: // QUOTED_ID
      case symbol_kind::S_BAD_ID: // BAD_ID
      case symbol_kind::S_HTML_BEGIN: // "<"
      case symbol_kind::S_HTML_END: // ">"
      case symbol_kind::S_DIR_EDGEOP: // DIR_EDGEOP
      case symbol_kind::S_UNDIR_EDGEOP: // UNDIR_EDGEOP
      case symbol_kind::S_HTML_CONTENT: // HTML_CONTENT
      case symbol_kind::S_LEX_ERROR: // LEX_ERROR
      case symbol_kind::S_id: // id
      case symbol_kind::S_html_id: // html_id
      case symbol_kind::S_html: // html
      case symbol_kind::S_edge_target: // edge_target
      case symbol_kind::S_node_id: // node_id
      case symbol_kind::S_subgraph: // subgraph
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_kvp: // kvp
        value.copy< AttributeKVP > (that.value);
        break;

      case symbol_kind::S_attr_list: // attr_list
      case symbol_kind::S_maybe_attr_list: // maybe_attr_list
      case symbol_kind::S_a_list: // a_list
        value.copy< AttributeStorage > (that.value);
        break;

      case symbol_kind::S_edge_rhs: // edge_rhs
        value.copy< std::list<std::string> > (that.value);
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_QUOTED_ID: // QUOTED_ID
      case symbol_kind::S_BAD_ID: // BAD_ID
      case symbol_kind::S_HTML_BEGIN: // "<"
      case symbol_kind::S_HTML_END: // ">"
      case symbol_kind::S_DIR_EDGEOP: // DIR_EDGEOP
      case symbol_kind::S_UNDIR_EDGEOP: // UNDIR_EDGEOP
      case symbol_kind::S_HTML_CONTENT: // HTML_CONTENT
      case symbol_kind::S_LEX_ERROR: // LEX_ERROR
      case symbol_kind::S_id: // id
      case symbol_kind::S_html_id: // html_id
      case symbol_kind::S_html: // html
      case symbol_kind::S_edge_target: // edge_target
      case symbol_kind::S_node_id: // node_id
      case symbol_kind::S_subgraph: // subgraph
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_kvp: // kvp
        value.move< AttributeKVP > (that.value);
        break;

      case symbol_kind::S_attr_list: // attr_list
      case symbol_kind::S_maybe_attr_list: // maybe_attr_list
      case symbol_kind::S_a_list: // a_list
        value.move< AttributeStorage > (that.value);
        break;

      case symbol_kind::S_edge_rhs: // edge_rhs
        value.move< std::list<std::string> > (that.value);
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_QUOTED_ID: // QUOTED_ID
      case symbol_kind::S_BAD_ID: // BAD_ID
      case symbol_kind::S_HTML_BEGIN: // "<"
      case symbol_kind::S_HTML_END: // ">"
      case symbol_kind::S_DIR_EDGEOP: // DIR_EDGEOP
      case symbol_kind::S_UNDIR_EDGEOP: // UNDIR_EDGEOP
      case symbol_kind::S_HTML_CONTENT: // HTML_CONTENT
      case symbol_kind::S_LEX_ERROR: // LEX_ERROR
      case symbol_kind::S_id: // id
      case symbol_kind::S_html_id: // html_id
      case symbol_kind::S_html: // html
      case symbol_kind::S_edge_target: // edge_target
      case symbol_kind::S_node_id: // node_id
      case symbol_kind::S_subgraph: // subgraph
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    // User initialization code.
#line 37 "dot.y"
{
    yyla.location.initialize(&d.getFileName()); // set the initial location's filename, so it shows up in error messages
}

#line 537 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (d, scanner, yyla));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_kvp: // kvp
        yylhs.value.emplace< AttributeKVP > ();
        break;

      case symbol_kind::S_attr_list: // attr_list
      case symbol_kind::S_maybe_attr_list: // maybe_attr_list
      case symbol_kind::S_a_list: // a_list
        yylhs.value.emplace< AttributeStorage > ();
        break;

      case symbol_kind::S_edge_rhs: // edge_rhs
        yylhs.value.emplace< std::list<std::string> > ();
        break;

      case symbol_kind::S_ID: // ID
      case symbol_kind::S_QUOTED_ID: // QUOTED_ID
      case symbol_kind::S_BAD_ID: // BAD_ID
      case symbol_kind::S_HTML_BEGIN: // "<"
      case symbol_kind::S_HTML_END: // ">"
      case symbol_kind::S_DIR_EDGEOP: // DIR_EDGEOP
      case symbol_kind::S_UNDIR_EDGEOP: // UNDIR_EDGEOP
      case symbol_kind::S_HTML_CONTENT: // HTML_CONTENT
      case symbol_kind::S_LEX_ERROR: // LEX_ERROR
      case symbol_kind::S_id: // id
      case symbol_kind::S_html_id: // html_id
      case symbol_kind::S_html: // html
      case symbol_kind::S_edge_target: // edge_target
      case symbol_kind::S_node_id: // node_id
      case symbol_kind::S_subgraph: // subgraph
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // graph: maybe_strict graph_type id "{" stmt_list "}"
#line 58 "dot.y"
                                                           { d.setGraphName(std::move(yystack_[3].value.as < std::string > ())); }
#line 707 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 4: // maybe_strict: STRICT
#line 62 "dot.y"
                       { error(yylhs.location, "strict keyword not implemented"); YYERROR; }
#line 713 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 6: // graph_type: GRAPH
#line 65 "dot.y"
                      { error(yylhs.location, "undirected graphs not implemented"); YYERROR; }
#line 719 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 16: // attr_stmt: GRAPH attr_list
#line 77 "dot.y"
                                { d.appendGraphAttributes(std::move(yystack_[0].value.as < AttributeStorage > ())); }
#line 725 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 17: // attr_stmt: NODE attr_list
#line 78 "dot.y"
                                { d.appendDefaultNodeAttributes(std::move(yystack_[0].value.as < AttributeStorage > ())); }
#line 731 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 18: // attr_stmt: EDGE attr_list
#line 79 "dot.y"
                                { d.appendDefaultEdgeAttributes(std::move(yystack_[0].value.as < AttributeStorage > ())); }
#line 737 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 19: // attr_list: "[" a_list "]" attr_list
#line 81 "dot.y"
                                       { set_all(yystack_[0].value.as < AttributeStorage > (),yystack_[2].value.as < AttributeStorage > ()); yylhs.value.as < AttributeStorage > () = yystack_[0].value.as < AttributeStorage > (); }
#line 743 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 20: // attr_list: "[" "]" attr_list
#line 82 "dot.y"
                                       { yylhs.value.as < AttributeStorage > () = yystack_[0].value.as < AttributeStorage > (); }
#line 749 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 21: // attr_list: "[" "]"
#line 83 "dot.y"
                                       { yylhs.value.as < AttributeStorage > () = AttributeStorage(); }
#line 755 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 22: // attr_list: "[" a_list "]"
#line 84 "dot.y"
                                       { yylhs.value.as < AttributeStorage > () = yystack_[1].value.as < AttributeStorage > (); }
#line 761 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 23: // maybe_attr_list: attr_list
#line 86 "dot.y"
                  { yylhs.value.as < AttributeStorage > () = yystack_[0].value.as < AttributeStorage > (); }
#line 767 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 24: // maybe_attr_list: %empty
#line 86 "dot.y"
                              { yylhs.value.as < AttributeStorage > () = AttributeStorage(); }
#line 773 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 25: // a_list: kvp maybe_alist_sep a_list
#line 88 "dot.y"
                                          { yystack_[0].value.as < AttributeStorage > ().setString(yystack_[2].value.as < AttributeKVP > ().first, yystack_[2].value.as < AttributeKVP > ().second); yylhs.value.as < AttributeStorage > () = yystack_[0].value.as < AttributeStorage > (); }
#line 779 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 26: // a_list: kvp
#line 89 "dot.y"
                                          { yylhs.value.as < AttributeStorage > () = AttributeStorage{yystack_[0].value.as < AttributeKVP > ()}; }
#line 785 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 30: // kvp: id
#line 93 "dot.y"
                         { yylhs.value.as < AttributeKVP > () = AttributeKVP(yystack_[0].value.as < std::string > (), "true"); }
#line 791 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 31: // kvp: id "=" id
#line 94 "dot.y"
                         { yylhs.value.as < AttributeKVP > () = AttributeKVP(yystack_[2].value.as < std::string > (),     yystack_[0].value.as < std::string > ()); }
#line 797 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 32: // id: ID
#line 96 "dot.y"
                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 803 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 33: // id: QUOTED_ID
#line 97 "dot.y"
                          {
                    yystack_[0].value.as < std::string > ().erase(0, 1); // remove first quote
                    yystack_[0].value.as < std::string > ().erase(yystack_[0].value.as < std::string > ().size()-1, 1); // remove last quote
                    // for each instance of \"
                    const auto next_bs_qu = [&](auto i) { return (std::ptrdiff_t)yystack_[0].value.as < std::string > ().find("\\\"", i); };
                    for (auto i = next_bs_qu(0); i != yystack_[0].value.as < std::string > ().npos; i = next_bs_qu(i+1)) {
                        yystack_[0].value.as < std::string > ().erase(i, 1); // remove the backslash
                    }
                    yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
                }
#line 818 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 34: // id: html_id
#line 107 "dot.y"
                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 824 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 35: // html_id: "<" html ">"
#line 110 "dot.y"
                                         { yylhs.value.as < std::string > () = yystack_[2].value.as < std::string > () + yystack_[1].value.as < std::string > () + yystack_[0].value.as < std::string > (); }
#line 830 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 36: // html: html_id html
#line 113 "dot.y"
                             { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + yystack_[0].value.as < std::string > (); }
#line 836 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 37: // html: HTML_CONTENT html
#line 114 "dot.y"
                                  { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > () + yystack_[0].value.as < std::string > (); }
#line 842 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 38: // html: %empty
#line 115 "dot.y"
                {}
#line 848 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 39: // edge_stmt: edge_target edge_rhs maybe_attr_list
#line 117 "dot.y"
                                                     {
                    auto lhs = yystack_[2].value.as < std::string > ();
                    for (const auto& rhs : yystack_[1].value.as < std::list<std::string> > ()) {
                        d.addEdge(lhs, rhs, yystack_[0].value.as < AttributeStorage > ());
                        lhs = rhs;
                    }
                }
#line 860 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 40: // edge_rhs: edgeop edge_target edge_rhs
#line 126 "dot.y"
                                            { yystack_[0].value.as < std::list<std::string> > ().push_front(yystack_[1].value.as < std::string > ()); yylhs.value.as < std::list<std::string> > () = yystack_[0].value.as < std::list<std::string> > (); }
#line 866 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 41: // edge_rhs: edgeop edge_target
#line 127 "dot.y"
                                   { yylhs.value.as < std::list<std::string> > () = { yystack_[0].value.as < std::string > () }; }
#line 872 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 42: // edge_target: id
#line 129 "dot.y"
                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 878 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 43: // edge_target: subgraph
#line 130 "dot.y"
                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 884 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 46: // node_stmt: node_id maybe_attr_list
#line 134 "dot.y"
                                        { d.addOrCreateNode(std::move(yystack_[1].value.as < std::string > ()), std::move(yystack_[0].value.as < AttributeStorage > ())); }
#line 890 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 47: // node_id: id
#line 136 "dot.y"
                { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 896 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 48: // node_id: id port
#line 137 "dot.y"
                        { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 902 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 51: // subgraph: SUBGRAPH id "{" stmt_list "}"
#line 143 "dot.y"
                                            { error(yylhs.location, "subgraphs not implemented"); YYERROR; yylhs.value.as < std::string > () = yystack_[3].value.as < std::string > (); }
#line 908 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 52: // subgraph: SUBGRAPH "{" stmt_list "}"
#line 144 "dot.y"
                                            { error(yylhs.location, "subgraphs not implemented"); YYERROR; yylhs.value.as < std::string > () = ""; }
#line 914 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;

  case 53: // subgraph: "{" stmt_list "}"
#line 145 "dot.y"
                                            { error(yylhs.location, "subgraphs not implemented"); YYERROR; yylhs.value.as < std::string > () = ""; }
#line 920 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"
    break;


#line 924 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -16;

  const signed char parser::yytable_ninf_ = -44;

  const signed char
  parser::yypact_[] =
  {
     -10,   -16,     3,    29,   -16,   -16,   -16,    54,   -16,   -16,
       0,    47,    -9,   -16,     0,     0,     5,    -4,    70,    -4,
      -4,    47,     2,    10,   -16,    28,   -16,    46,   -16,    -4,
      57,    47,   -16,   -16,   -16,    15,   -16,    47,    17,   -16,
     -16,    61,   -16,    47,   -16,    27,    27,   -16,   -16,   -16,
      -4,    66,   -16,   -16,    68,    -4,    25,    55,    63,    80,
      47,   -16,   -16,   -16,    75,   -16,   -16,    46,   -16,   -16,
     -16,    -4,   -16,   -16,    27,    27,   -16,    82,    62,   -16,
     -16,   -16,   -16,   -16,   -16,   -16,   -16,   -16,   -16,   -16,
     -16,   -16,   -16,   -16,   -16
  };

  const signed char
  parser::yydefact_[] =
  {
       5,     4,     0,     0,     1,     6,     7,     0,    32,    33,
      38,    10,     0,    34,    38,    38,     0,     0,     0,     0,
       0,    10,     0,    10,    13,    47,    12,     0,    11,    24,
      15,    10,    37,    36,    35,     0,    16,    10,     0,    17,
      18,     0,     3,    10,     9,     0,     0,    48,    45,    44,
      24,     0,    23,    46,     0,    21,     0,    29,    30,     0,
      10,    53,     8,    14,    49,    39,    42,    41,    43,     2,
      20,    22,    28,    27,     0,     0,    52,     0,     0,    40,
      19,    25,    31,    51,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    50
  };

  const signed char
  parser::yypgoto_[] =
  {
     -16,   -16,   -16,   -16,   -14,   -16,   -16,   -15,    35,    30,
     -16,   -16,    -7,    33,    67,   -16,    34,    51,   -16,   -16,
     -16,   -16,    52,   -16
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     2,     3,     7,    22,    23,    24,    52,    53,    56,
      74,    57,    25,    13,    16,    26,    50,    27,    51,    28,
      29,    47,    30,    94
  };

  const signed char
  parser::yytable_[] =
  {
      12,     1,    36,     4,    39,    40,    10,    41,    31,    44,
      14,    38,    34,     8,     9,    35,    10,    54,     8,     9,
      42,    10,    17,    59,    18,    19,    20,    21,    58,    62,
       8,     9,    43,    10,    60,    55,   -42,   -42,    63,    64,
      70,     5,     6,    15,    66,    71,    77,    15,    15,    45,
       8,     9,    46,    10,    48,    49,    80,     8,     9,    17,
      10,    18,    19,    20,    21,   -43,   -43,    58,    82,     8,
       9,    11,    10,     8,     9,   -26,    10,    72,    73,    61,
      18,    32,    33,    21,    75,    65,    69,    37,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    76,    78,
      83,    79,    67,    68,    81
  };

  const signed char
  parser::yycheck_[] =
  {
       7,    11,    17,     0,    19,    20,     6,    21,    17,    23,
      10,    18,     7,     3,     4,    19,     6,    31,     3,     4,
      18,     6,    12,    37,    14,    15,    16,    17,    35,    43,
       3,     4,    22,     6,    17,    20,     8,     9,    45,    46,
      55,    12,    13,    10,    51,    20,    60,    14,    15,    21,
       3,     4,    24,     6,     8,     9,    71,     3,     4,    12,
       6,    14,    15,    16,    17,     8,     9,    74,    75,     3,
       4,    17,     6,     3,     4,    20,     6,    22,    23,    18,
      14,    14,    15,    17,    21,    50,    18,    17,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    18,    24,
      18,    67,    51,    51,    74
  };

  const signed char
  parser::yystos_[] =
  {
       0,    11,    37,    38,     0,    12,    13,    39,     3,     4,
       6,    17,    48,    49,    10,    49,    50,    12,    14,    15,
      16,    17,    40,    41,    42,    48,    51,    53,    55,    56,
      58,    17,    50,    50,     7,    19,    43,    17,    48,    43,
      43,    40,    18,    22,    40,    21,    24,    57,     8,     9,
      52,    54,    43,    44,    40,    20,    45,    47,    48,    40,
      17,    18,    40,    48,    48,    44,    48,    53,    58,    18,
      43,    20,    22,    23,    46,    21,    18,    40,    24,    52,
      43,    45,    48,    18,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    59
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    36,    37,    37,    38,    38,    39,    39,    40,    40,
      40,    41,    41,    41,    41,    41,    42,    42,    42,    43,
      43,    43,    43,    44,    44,    45,    45,    46,    46,    46,
      47,    47,    48,    48,    48,    49,    50,    50,    50,    51,
      52,    52,    53,    53,    54,    54,    55,    56,    56,    57,
      57,    58,    58,    58,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     6,     5,     1,     0,     1,     1,     3,     2,
       0,     1,     1,     1,     3,     1,     2,     2,     2,     4,
       3,     2,     3,     1,     0,     3,     1,     1,     1,     0,
       1,     3,     1,     1,     1,     3,     2,     2,     0,     3,
       3,     2,     1,     1,     1,     1,     2,     1,     2,     2,
       4,     5,     4,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "ID", "QUOTED_ID",
  "BAD_ID", "\"<\"", "\">\"", "DIR_EDGEOP", "UNDIR_EDGEOP", "HTML_CONTENT",
  "STRICT", "GRAPH", "DIGRAPH", "SUBGRAPH", "NODE", "EDGE", "\"{\"",
  "\"}\"", "\"[\"", "\"]\"", "\"=\"", "\";\"", "\",\"", "\":\"",
  "LEX_ERROR", "\"n\"", "\"ne\"", "\"e\"", "\"se\"", "\"s\"", "\"sw\"",
  "\"w\"", "\"nw\"", "\"c\"", "\"_\"", "$accept", "graph", "maybe_strict",
  "graph_type", "stmt_list", "stmt", "attr_stmt", "attr_list",
  "maybe_attr_list", "a_list", "maybe_alist_sep", "kvp", "id", "html_id",
  "html", "edge_stmt", "edge_rhs", "edge_target", "edgeop", "node_stmt",
  "node_id", "port", "subgraph", "compas_pt", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,    58,    58,    59,    62,    62,    65,    65,    67,    68,
      69,    71,    72,    73,    74,    75,    77,    78,    79,    81,
      82,    83,    84,    86,    86,    88,    89,    91,    91,    91,
      93,    94,    96,    97,   107,   110,   113,   114,   115,   117,
     126,   127,   129,   130,   132,   132,   134,   136,   137,   139,
     140,   143,   144,   145,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1460 "/home/joemar/Documents/courses/csci5204-fa25/CGRA-pathfinding/CGRA/cgra-me-release-v2.0.1/src/dotparser/generated/dot.tab.cc"

#line 149 "dot.y"


void yy::parser::error (const location_type& l, const std::string& m)
{
    throw d.makeError(l, m);
}
