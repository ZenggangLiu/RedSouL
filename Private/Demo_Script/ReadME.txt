1. 选择语句：
  Parse_S()
  {
      look_ahead:
      | a:          // 表示look_ahead为a
        Parse_A();
        MatchToken(a);
        Parse_S();
      | b, c:       // 表示look_ahead为b或者c
        Parse_B()
        MatchToken(b)
        Parse_S()
      | d:          // 表示look_ahead为d
        MatchToke(d)    
      |: error()    // 表示look_ahead为其它数值
  }