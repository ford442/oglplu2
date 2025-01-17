/// Copyright Matus Chochlik.
/// Distributed under the Boost Software License, Version 1.0.
/// See accompanying file LICENSE_1_0.txt or copy at
///  http://www.boost.org/LICENSE_1_0.txt
///
#include <eagine/math/vector.hpp>
#define BOOST_TEST_MODULE EAGINE_math_vector
#include "../unit_test_begin.inl"

BOOST_AUTO_TEST_SUITE(math_vector_tests)

static eagine::test_random_generator rg;

template <typename T, bool V>
void test_math_vector_default_ctr_TV() {
    eagine::math::vector<T, 1, V> v1;
    EAGINE_MAYBE_UNUSED(v1);
    eagine::math::vector<T, 2, V> v2;
    EAGINE_MAYBE_UNUSED(v2);
    eagine::math::vector<T, 3, V> v3;
    EAGINE_MAYBE_UNUSED(v3);
    eagine::math::vector<T, 4, V> v4;
    EAGINE_MAYBE_UNUSED(v4);
    eagine::math::vector<T, 5, V> v5;
    EAGINE_MAYBE_UNUSED(v5);
    eagine::math::vector<T, 6, V> v6;
    EAGINE_MAYBE_UNUSED(v6);
    eagine::math::vector<T, 7, V> v7;
    EAGINE_MAYBE_UNUSED(v7);
    eagine::math::vector<T, 8, V> v8;
    EAGINE_MAYBE_UNUSED(v8);
    eagine::math::vector<T, 11, V> v11;
    EAGINE_MAYBE_UNUSED(v11);
    eagine::math::vector<T, 17, V> v17;
    EAGINE_MAYBE_UNUSED(v17);
    eagine::math::vector<T, 23, V> v23;
    EAGINE_MAYBE_UNUSED(v23);
}

template <typename T>
void test_math_vector_default_ctr_T() {
    test_math_vector_default_ctr_TV<T, true>();
    test_math_vector_default_ctr_TV<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_default_ctr) {
    test_math_vector_default_ctr_T<int>();
    test_math_vector_default_ctr_T<float>();
    test_math_vector_default_ctr_T<double>();
}

template <typename T, int N, bool V>
void test_math_vector_zero_TNV() {
    auto v = eagine::math::vector<T, N, V>::zero();
    for(int i = 0; i < N; ++i) {
        BOOST_CHECK_EQUAL(v[i], T(0));
    }
}

template <typename T, bool V>
void test_math_vector_zero_TV() {
    test_math_vector_zero_TNV<T, 1, V>();
    test_math_vector_zero_TNV<T, 2, V>();
    test_math_vector_zero_TNV<T, 3, V>();
    test_math_vector_zero_TNV<T, 4, V>();
    test_math_vector_zero_TNV<T, 5, V>();
    test_math_vector_zero_TNV<T, 6, V>();
    test_math_vector_zero_TNV<T, 7, V>();
    test_math_vector_zero_TNV<T, 8, V>();
    test_math_vector_zero_TNV<T, 11, V>();
    test_math_vector_zero_TNV<T, 17, V>();
    test_math_vector_zero_TNV<T, 31, V>();
}

template <typename T>
void test_math_vector_zero_T() {
    test_math_vector_zero_TV<T, true>();
    test_math_vector_zero_TV<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_zero) {
    test_math_vector_zero_T<int>();
    test_math_vector_zero_T<float>();
    test_math_vector_zero_T<double>();
}

template <typename T, int N, bool V>
void test_math_vector_fill_TNV() {
    T r = rg.get_any<T>();
    auto v = eagine::math::vector<T, N, V>::fill(r);
    for(int i = 0; i < N; ++i) {
        BOOST_CHECK_EQUAL(v[i], r);
    }
}

template <typename T, bool V>
void test_math_vector_fill_TV() {
    test_math_vector_fill_TNV<T, 1, V>();
    test_math_vector_fill_TNV<T, 2, V>();
    test_math_vector_fill_TNV<T, 3, V>();
    test_math_vector_fill_TNV<T, 4, V>();
    test_math_vector_fill_TNV<T, 5, V>();
    test_math_vector_fill_TNV<T, 6, V>();
    test_math_vector_fill_TNV<T, 7, V>();
    test_math_vector_fill_TNV<T, 8, V>();
    test_math_vector_fill_TNV<T, 11, V>();
    test_math_vector_fill_TNV<T, 17, V>();
    test_math_vector_fill_TNV<T, 31, V>();
}

template <typename T>
void test_math_vector_fill_T() {
    test_math_vector_fill_TV<T, true>();
    test_math_vector_fill_TV<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_fill) {
    test_math_vector_fill_T<int>();
    test_math_vector_fill_T<float>();
    test_math_vector_fill_T<double>();
}

template <typename T, int N, bool V>
void test_math_vector_axis_TNV() {
    for(int j = 0; j < N; ++j) {
        T r = rg.get_any<T>();
        auto v = eagine::math::vector<T, N, V>::axis(j, r);
        for(int i = 0; i < N; ++i) {
            BOOST_CHECK_EQUAL(v[i], j == i ? r : T(0));
        }
    }
}

template <typename T, bool V>
void test_math_vector_axis_TV() {
    test_math_vector_axis_TNV<T, 1, V>();
    test_math_vector_axis_TNV<T, 2, V>();
    test_math_vector_axis_TNV<T, 3, V>();
    test_math_vector_axis_TNV<T, 4, V>();
    test_math_vector_axis_TNV<T, 5, V>();
    test_math_vector_axis_TNV<T, 6, V>();
    test_math_vector_axis_TNV<T, 7, V>();
    test_math_vector_axis_TNV<T, 8, V>();
    test_math_vector_axis_TNV<T, 11, V>();
    test_math_vector_axis_TNV<T, 17, V>();
    test_math_vector_axis_TNV<T, 31, V>();
}

template <typename T>
void test_math_vector_axis_T() {
    test_math_vector_axis_TV<T, true>();
    test_math_vector_axis_TV<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_axis_i) {
    test_math_vector_axis_T<int>();
    test_math_vector_axis_T<float>();
    test_math_vector_axis_T<double>();
}

template <typename T, int I, int N, bool V>
void test_math_vector_axis_TINV() {
    T r = rg.template get_any<T>();

    auto v = eagine::math::vector<T, N, V>::template axis<I>(r);
    for(int i = 0; i < N; ++i) {
        BOOST_CHECK_EQUAL(v[i], i == I ? r : T(0));
    }
}

template <typename T, bool V>
void test_math_vector_axis_TV_I() {
    test_math_vector_axis_TINV<T, 0, 1, V>();

    test_math_vector_axis_TINV<T, 0, 2, V>();
    test_math_vector_axis_TINV<T, 1, 2, V>();

    test_math_vector_axis_TINV<T, 0, 3, V>();
    test_math_vector_axis_TINV<T, 1, 3, V>();
    test_math_vector_axis_TINV<T, 2, 3, V>();

    test_math_vector_axis_TINV<T, 0, 4, V>();
    test_math_vector_axis_TINV<T, 1, 4, V>();
    test_math_vector_axis_TINV<T, 2, 4, V>();
    test_math_vector_axis_TINV<T, 3, 4, V>();

    test_math_vector_axis_TINV<T, 0, 5, V>();
    test_math_vector_axis_TINV<T, 1, 5, V>();
    test_math_vector_axis_TINV<T, 2, 5, V>();
    test_math_vector_axis_TINV<T, 3, 5, V>();
    test_math_vector_axis_TINV<T, 4, 5, V>();

    test_math_vector_axis_TINV<T, 0, 6, V>();
    test_math_vector_axis_TINV<T, 1, 6, V>();
    test_math_vector_axis_TINV<T, 2, 6, V>();
    test_math_vector_axis_TINV<T, 3, 6, V>();
    test_math_vector_axis_TINV<T, 4, 6, V>();
    test_math_vector_axis_TINV<T, 5, 6, V>();

    test_math_vector_axis_TINV<T, 0, 7, V>();
    test_math_vector_axis_TINV<T, 1, 7, V>();
    test_math_vector_axis_TINV<T, 2, 6, V>();
    test_math_vector_axis_TINV<T, 3, 7, V>();
    test_math_vector_axis_TINV<T, 4, 7, V>();
    test_math_vector_axis_TINV<T, 5, 7, V>();
    test_math_vector_axis_TINV<T, 6, 7, V>();

    test_math_vector_axis_TINV<T, 0, 8, V>();
    test_math_vector_axis_TINV<T, 1, 8, V>();
    test_math_vector_axis_TINV<T, 2, 8, V>();
    test_math_vector_axis_TINV<T, 3, 8, V>();
    test_math_vector_axis_TINV<T, 4, 8, V>();
    test_math_vector_axis_TINV<T, 5, 8, V>();
    test_math_vector_axis_TINV<T, 6, 8, V>();
    test_math_vector_axis_TINV<T, 7, 8, V>();
}

template <typename T>
void test_math_vector_axis_T_I() {
    test_math_vector_axis_TV_I<T, true>();
    test_math_vector_axis_TV_I<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_axis_I) {
    test_math_vector_axis_T_I<int>();
    test_math_vector_axis_T_I<float>();
    test_math_vector_axis_T_I<double>();
}

template <typename T, bool V>
void test_math_vector_make_and_elem_TV() {
    T r1 = rg.template get_any<T>();
    auto v1 = eagine::math::vector<T, 1, V>::make(r1);
    BOOST_CHECK_EQUAL(v1[0], r1);

    T r2 = rg.template get_any<T>();
    auto v2 = eagine::math::vector<T, 2, V>::make(r2, r1);
    BOOST_CHECK_EQUAL(v2[0], r2);
    BOOST_CHECK_EQUAL(v2[1], r1);

    T r3 = rg.template get_any<T>();
    auto v3 = eagine::math::vector<T, 3, V>::make(r3, r2, r1);
    BOOST_CHECK_EQUAL(v3[0], r3);
    BOOST_CHECK_EQUAL(v3[1], r2);
    BOOST_CHECK_EQUAL(v3[2], r1);

    T r4 = rg.template get_any<T>();
    auto v4 = eagine::math::vector<T, 4, V>::make(r4, r3, r2, r1);
    BOOST_CHECK_EQUAL(v4[0], r4);
    BOOST_CHECK_EQUAL(v4[1], r3);
    BOOST_CHECK_EQUAL(v4[2], r2);
    BOOST_CHECK_EQUAL(v4[3], r1);

    T r5 = rg.template get_any<T>();
    auto v5 = eagine::math::vector<T, 5, V>::make(r5, r4, r3, r2, r1);
    BOOST_CHECK_EQUAL(v5[0], r5);
    BOOST_CHECK_EQUAL(v5[1], r4);
    BOOST_CHECK_EQUAL(v5[2], r3);
    BOOST_CHECK_EQUAL(v5[3], r2);
    BOOST_CHECK_EQUAL(v5[4], r1);

    T r6 = rg.template get_any<T>();
    auto v6 = eagine::math::vector<T, 6, V>::make(r6, r5, r4, r3, r2, r1);
    BOOST_CHECK_EQUAL(v6[0], r6);
    BOOST_CHECK_EQUAL(v6[1], r5);
    BOOST_CHECK_EQUAL(v6[2], r4);
    BOOST_CHECK_EQUAL(v6[3], r3);
    BOOST_CHECK_EQUAL(v6[4], r2);
    BOOST_CHECK_EQUAL(v6[5], r1);

    T r7 = rg.template get_any<T>();
    auto v7 = eagine::math::vector<T, 7, V>::make(r7, r6, r5, r4, r3, r2, r1);
    BOOST_CHECK_EQUAL(v7[0], r7);
    BOOST_CHECK_EQUAL(v7[1], r6);
    BOOST_CHECK_EQUAL(v7[2], r5);
    BOOST_CHECK_EQUAL(v7[3], r4);
    BOOST_CHECK_EQUAL(v7[4], r3);
    BOOST_CHECK_EQUAL(v7[5], r2);
    BOOST_CHECK_EQUAL(v7[6], r1);

    T r8 = rg.template get_any<T>();
    auto v8 =
      eagine::math::vector<T, 8, V>::make(r8, r7, r6, r5, r4, r3, r2, r1);
    BOOST_CHECK_EQUAL(v8[0], r8);
    BOOST_CHECK_EQUAL(v8[1], r7);
    BOOST_CHECK_EQUAL(v8[2], r6);
    BOOST_CHECK_EQUAL(v8[3], r5);
    BOOST_CHECK_EQUAL(v8[4], r4);
    BOOST_CHECK_EQUAL(v8[5], r3);
    BOOST_CHECK_EQUAL(v8[6], r2);
    BOOST_CHECK_EQUAL(v8[7], r1);
}

template <typename T>
void test_math_vector_make_and_elem_T() {
    test_math_vector_make_and_elem_TV<T, true>();
    test_math_vector_make_and_elem_TV<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_make_and_elem) {
    test_math_vector_make_and_elem_T<int>();
    test_math_vector_make_and_elem_T<float>();
    test_math_vector_make_and_elem_T<double>();
}

template <typename T, bool V>
void test_math_vector_from_and_elem_TV() {
    T r1 = rg.get_any<T>();
    auto v10 = eagine::math::vector<T, 1, V>::make(r1);
    BOOST_CHECK_EQUAL(v10[0], r1);

    T r2 = rg.get_any<T>();
    auto v21 = eagine::math::vector<T, 2, V>::from(v10, r2);
    BOOST_CHECK_EQUAL(v21[0], r1);
    BOOST_CHECK_EQUAL(v21[1], r2);

    T r3 = rg.get_any<T>();
    auto v32 = eagine::math::vector<T, 3, V>::from(v21, r3);
    BOOST_CHECK_EQUAL(v32[0], r1);
    BOOST_CHECK_EQUAL(v32[1], r2);
    BOOST_CHECK_EQUAL(v32[2], r3);

    auto v31 = eagine::math::vector<T, 3, V>::from(v10, r3);
    BOOST_CHECK_EQUAL(v31[0], r1);
    BOOST_CHECK_EQUAL(v31[1], r3);
    BOOST_CHECK_EQUAL(v31[2], r3);

    T r4 = rg.get_any<T>();
    auto v43 = eagine::math::vector<T, 4, V>::from(v32, r4);
    BOOST_CHECK_EQUAL(v43[0], r1);
    BOOST_CHECK_EQUAL(v43[1], r2);
    BOOST_CHECK_EQUAL(v43[2], r3);
    BOOST_CHECK_EQUAL(v43[3], r4);

    auto v42 = eagine::math::vector<T, 4, V>::from(v21, r4);
    BOOST_CHECK_EQUAL(v42[0], r1);
    BOOST_CHECK_EQUAL(v42[1], r2);
    BOOST_CHECK_EQUAL(v42[2], r4);
    BOOST_CHECK_EQUAL(v42[3], r4);

    auto v41 = eagine::math::vector<T, 4, V>::from(v10, r4);
    BOOST_CHECK_EQUAL(v41[0], r1);
    BOOST_CHECK_EQUAL(v41[1], r4);
    BOOST_CHECK_EQUAL(v41[2], r4);
    BOOST_CHECK_EQUAL(v41[3], r4);

    T r5 = rg.get_any<T>();
    auto v54 = eagine::math::vector<T, 5, V>::from(v43, r5);
    BOOST_CHECK_EQUAL(v54[0], r1);
    BOOST_CHECK_EQUAL(v54[1], r2);
    BOOST_CHECK_EQUAL(v54[2], r3);
    BOOST_CHECK_EQUAL(v54[3], r4);
    BOOST_CHECK_EQUAL(v54[4], r5);

    auto v53 = eagine::math::vector<T, 5, V>::from(v32, r5);
    BOOST_CHECK_EQUAL(v53[0], r1);
    BOOST_CHECK_EQUAL(v53[1], r2);
    BOOST_CHECK_EQUAL(v53[2], r3);
    BOOST_CHECK_EQUAL(v53[3], r5);
    BOOST_CHECK_EQUAL(v53[3], r5);

    auto v52 = eagine::math::vector<T, 5, V>::from(v21, r5);
    BOOST_CHECK_EQUAL(v52[0], r1);
    BOOST_CHECK_EQUAL(v52[1], r2);
    BOOST_CHECK_EQUAL(v52[2], r5);
    BOOST_CHECK_EQUAL(v52[3], r5);
    BOOST_CHECK_EQUAL(v52[4], r5);

    auto v51 = eagine::math::vector<T, 5, V>::from(v10, r5);
    BOOST_CHECK_EQUAL(v51[0], r1);
    BOOST_CHECK_EQUAL(v51[1], r5);
    BOOST_CHECK_EQUAL(v51[2], r5);
    BOOST_CHECK_EQUAL(v51[3], r5);
    BOOST_CHECK_EQUAL(v51[4], r5);
}

template <typename T>
void test_math_vector_from_and_elem_T() {
    test_math_vector_from_and_elem_TV<T, true>();
    test_math_vector_from_and_elem_TV<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_from_and_elem) {
    test_math_vector_from_and_elem_T<int>();
    test_math_vector_from_and_elem_T<float>();
    test_math_vector_from_and_elem_T<double>();
}

template <typename T, bool V>
void test_math_vector_from2_and_elem_TV() {
    T ra = rg.get_any<T>();
    auto v1a = eagine::math::vector<T, 1, V>::make(ra);
    BOOST_CHECK_EQUAL(v1a[0], ra);

    T rb = rg.get_any<T>();
    auto v1b = eagine::math::vector<T, 1, V>::make(rb);
    BOOST_CHECK_EQUAL(v1b[0], rb);

    auto v2aa = eagine::math::vector<T, 2, V>::from(v1a, v1a);
    BOOST_CHECK_EQUAL(v2aa[0], ra);
    BOOST_CHECK_EQUAL(v2aa[1], ra);

    auto v2ab = eagine::math::vector<T, 2, V>::from(v1a, v1b);
    BOOST_CHECK_EQUAL(v2ab[0], ra);
    BOOST_CHECK_EQUAL(v2ab[1], rb);

    auto v2bb = eagine::math::vector<T, 2, V>::from(v1b, v1b);
    BOOST_CHECK_EQUAL(v2bb[0], rb);
    BOOST_CHECK_EQUAL(v2bb[1], rb);

    auto v3aba = eagine::math::vector<T, 3, V>::from(v2ab, v1a);
    BOOST_CHECK_EQUAL(v3aba[0], ra);
    BOOST_CHECK_EQUAL(v3aba[1], rb);
    BOOST_CHECK_EQUAL(v3aba[2], ra);

    auto v3bab = eagine::math::vector<T, 3, V>::from(v1b, v2ab);
    BOOST_CHECK_EQUAL(v3bab[0], rb);
    BOOST_CHECK_EQUAL(v3bab[1], ra);
    BOOST_CHECK_EQUAL(v3bab[2], rb);

    auto v4abab = eagine::math::vector<T, 4, V>::from(v1a, v3bab);
    BOOST_CHECK_EQUAL(v4abab[0], ra);
    BOOST_CHECK_EQUAL(v4abab[1], rb);
    BOOST_CHECK_EQUAL(v4abab[2], ra);
    BOOST_CHECK_EQUAL(v4abab[3], rb);

    auto v4aabb = eagine::math::vector<T, 4, V>::from(v2aa, v2bb);
    BOOST_CHECK_EQUAL(v4aabb[0], ra);
    BOOST_CHECK_EQUAL(v4aabb[1], ra);
    BOOST_CHECK_EQUAL(v4aabb[2], rb);
    BOOST_CHECK_EQUAL(v4aabb[3], rb);

    auto v4baba = eagine::math::vector<T, 4, V>::from(v3bab, v1a);
    BOOST_CHECK_EQUAL(v4baba[0], rb);
    BOOST_CHECK_EQUAL(v4baba[1], ra);
    BOOST_CHECK_EQUAL(v4baba[2], rb);
    BOOST_CHECK_EQUAL(v4baba[3], ra);

    auto v5ababa = eagine::math::vector<T, 5, V>::from(v1a, v4baba);
    BOOST_CHECK_EQUAL(v5ababa[0], ra);
    BOOST_CHECK_EQUAL(v5ababa[1], rb);
    BOOST_CHECK_EQUAL(v5ababa[2], ra);
    BOOST_CHECK_EQUAL(v5ababa[3], rb);
    BOOST_CHECK_EQUAL(v5ababa[4], ra);

    auto v5aabab = eagine::math::vector<T, 5, V>::from(v2aa, v3bab);
    BOOST_CHECK_EQUAL(v5aabab[0], ra);
    BOOST_CHECK_EQUAL(v5aabab[1], ra);
    BOOST_CHECK_EQUAL(v5aabab[2], rb);
    BOOST_CHECK_EQUAL(v5aabab[3], ra);
    BOOST_CHECK_EQUAL(v5aabab[4], rb);

    auto v5ababb = eagine::math::vector<T, 5, V>::from(v3aba, v2bb);
    BOOST_CHECK_EQUAL(v5ababb[0], ra);
    BOOST_CHECK_EQUAL(v5ababb[1], rb);
    BOOST_CHECK_EQUAL(v5ababb[2], ra);
    BOOST_CHECK_EQUAL(v5ababb[3], rb);
    BOOST_CHECK_EQUAL(v5ababb[4], rb);

    auto v5babab = eagine::math::vector<T, 5, V>::from(v4baba, v1b);
    BOOST_CHECK_EQUAL(v5babab[0], rb);
    BOOST_CHECK_EQUAL(v5babab[1], ra);
    BOOST_CHECK_EQUAL(v5babab[2], rb);
    BOOST_CHECK_EQUAL(v5babab[3], ra);
    BOOST_CHECK_EQUAL(v5babab[4], rb);
}

template <typename T>
void test_math_vector_from2_and_elem_T() {
    test_math_vector_from2_and_elem_TV<T, true>();
    test_math_vector_from2_and_elem_TV<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_from2_and_elem) {
    test_math_vector_from2_and_elem_T<int>();
    test_math_vector_from2_and_elem_T<float>();
    test_math_vector_from2_and_elem_T<double>();
}

template <typename T, int N, bool V>
void test_math_vector_from3_and_elem_TNV() {
    T a[N];
    for(int i = 0; i < N; ++i) {
        a[i] = rg.get_any<T>();
    }

    auto v = eagine::math::vector<T, N, V>::from(a, N);

    for(int i = 0; i < N; ++i) {
        BOOST_CHECK_EQUAL(v[i], a[i]);
    }
}

template <typename T, bool V>
void test_math_vector_from3_and_elem_TV() {
    test_math_vector_from3_and_elem_TNV<T, 1, V>();
    test_math_vector_from3_and_elem_TNV<T, 2, V>();
    test_math_vector_from3_and_elem_TNV<T, 3, V>();
    test_math_vector_from3_and_elem_TNV<T, 4, V>();
    test_math_vector_from3_and_elem_TNV<T, 5, V>();
    test_math_vector_from3_and_elem_TNV<T, 6, V>();
    test_math_vector_from3_and_elem_TNV<T, 7, V>();
    test_math_vector_from3_and_elem_TNV<T, 8, V>();
    test_math_vector_from3_and_elem_TNV<T, 12, V>();
    test_math_vector_from3_and_elem_TNV<T, 15, V>();
    test_math_vector_from3_and_elem_TNV<T, 20, V>();
}

template <typename T>
void test_math_vector_from3_and_elem_T() {
    test_math_vector_from3_and_elem_TV<T, true>();
    test_math_vector_from3_and_elem_TV<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_from3_and_elem) {
    test_math_vector_from3_and_elem_T<int>();
    test_math_vector_from3_and_elem_T<float>();
    test_math_vector_from3_and_elem_T<double>();
}

template <typename T, int N, bool V>
void test_math_vector_from4_and_elem_TNV() {
    for(int j = 0; j < N; ++j) {
        T a[N];
        for(int i = 0; i < N - j; ++i) {
            a[i] = rg.get_any<T>();
        }

        T r = rg.get_any<T>();

        auto v = eagine::math::vector<T, N, V>::from(a, N - j, r);

        for(int i = 0; i < N - j; ++i) {
            BOOST_CHECK_EQUAL(v[i], a[i]);
        }

        for(int i = N - j; i < N; ++i) {
            BOOST_CHECK_EQUAL(v[i], r);
        }
    }
}

template <typename T, bool V>
void test_math_vector_from4_and_elem_TV() {
    test_math_vector_from4_and_elem_TNV<T, 1, V>();
    test_math_vector_from4_and_elem_TNV<T, 2, V>();
    test_math_vector_from4_and_elem_TNV<T, 3, V>();
    test_math_vector_from4_and_elem_TNV<T, 4, V>();
    test_math_vector_from4_and_elem_TNV<T, 5, V>();
    test_math_vector_from4_and_elem_TNV<T, 6, V>();
    test_math_vector_from4_and_elem_TNV<T, 7, V>();
    test_math_vector_from4_and_elem_TNV<T, 8, V>();
    test_math_vector_from4_and_elem_TNV<T, 13, V>();
    test_math_vector_from4_and_elem_TNV<T, 16, V>();
    test_math_vector_from4_and_elem_TNV<T, 22, V>();
}

template <typename T>
void test_math_vector_from4_and_elem_T() {
    test_math_vector_from4_and_elem_TV<T, true>();
    test_math_vector_from4_and_elem_TV<T, false>();
}

BOOST_AUTO_TEST_CASE(math_vector_from4_and_elem) {
    test_math_vector_from4_and_elem_T<int>();
    test_math_vector_from4_and_elem_T<float>();
    test_math_vector_from4_and_elem_T<double>();
}

// TODO

BOOST_AUTO_TEST_SUITE_END()

#include "../unit_test_end.inl"
