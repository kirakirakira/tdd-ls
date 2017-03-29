/*!
 * @file
 * @brief User-defined assert mechanism
 */

#ifndef UASSERT_TEST_H
#define UASSERT_TEST_H

#define CHECK_ASSERTION_FAILED(code) \
   uassert_ExpectAssertion(); \
   try { code; } catch(...) { } \
   CHECK(uassert_Asserted())

extern void uassert_ExpectAssertion(void);
extern bool uassert_Asserted(void);

#endif
