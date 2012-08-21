PHP_ARG_ENABLE(cpptest, whether to enable cpptest support,
Make sure that the comment is aligned:
[  --enable-cpptest           Enable cpptest support])

if test "$PHP_CPPTEST" != "no"; then
  PHP_REQUIRE_CXX()
  PHP_SUBST(CPPTEST_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, CPPTEST_SHARED_LIBADD)
  PHP_NEW_EXTENSION(cpptest, cpptest.cpp, $ext_shared)
fi
