

add_lldb_tool(lldb-analyze
    TestTools.cpp
    ABSTarget.cpp
    ABSDebugger.cpp
    ABSBreakpoint.cpp

    LINK_LIBS
    liblldb

    LINK_COMPONENTS
    Support
  )

