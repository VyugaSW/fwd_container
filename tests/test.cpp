#include <gtest/gtest.h>
#include <algorithm>
#include "stack.h"
#include "queue.h"

TEST(StackTest, Stack_Iterator)
{
    Stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    Stack<int>::const_iterator cit = s.cbegin(), ocit;
    EXPECT_EQ(*cit, 30);
    ocit = ++cit;
    EXPECT_EQ(*cit, 20);
    EXPECT_EQ(*ocit, 20);
    ocit = cit++;
    EXPECT_EQ(*cit, 10);
    EXPECT_EQ(*ocit, 20);
    ++cit;
    EXPECT_EQ(cit, s.cend());

    const Stack<int>& r = s;
    cit = r.begin();
    EXPECT_EQ(*cit, 30);
    ++cit;
    EXPECT_EQ(*cit, 20);
    cit++;
    EXPECT_EQ(*cit, 10);
    ++cit;
    EXPECT_EQ(cit, r.end());

    Stack<int>::iterator it = s.begin(), oit;
    EXPECT_EQ(*it, 30);
    oit = ++it;
    EXPECT_EQ(*it, 20);
    EXPECT_EQ(*oit, 20);
    oit = it++;
    EXPECT_EQ(*it, 10);
    EXPECT_EQ(*oit, 20);
    *oit = 5;
    EXPECT_EQ(*it, 10);
    EXPECT_EQ(*oit, 5);
    ++it;
    EXPECT_EQ(it, s.end());

    std::stringstream sout;
    sout << s;
    EXPECT_EQ(sout.str(), "30 5 10");
}

TEST(StackTest, Stack_For)
{
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3); 
    
    const Stack<int>& r = s;
    int expected1[] = {3, 2, 1};
    int idx = 0;
    for (auto& v : r) EXPECT_EQ(v, expected1[idx++]);

    int expected2[] = {6, 4, 2}; //{6, 5, 4}; //<-----  WHY??????
    idx = 0;
    for (auto& v : s) v *= 2; // Stack before: (3, 2, 1); Stack after: (6, 4, 2)

    idx = 0;
    for (const auto& v : s) EXPECT_EQ(v, expected2[idx++]);

    int expected3[] = {7, 5, 3}; // {7, 6, 5}; //<-----   WHY???????????????? 

    for (Stack<int>::iterator it = s.begin(); it != s.end(); ++it) {
        *it += 1;
    } //Stack before: (6, 4, 2); Stack after: (7, 5, 3); 
 

    idx = 0;
    for (Stack<int>::const_iterator it = s.begin(); it != s.end(); ++it)
        EXPECT_EQ(*it, expected3[idx++]);
    idx = 0;
    for (Stack<int>::const_iterator it = r.begin(); it != r.end(); ++it)
        EXPECT_EQ(*it, expected3[idx++]);
    idx = 0;
    for (Stack<int>::const_iterator it = s.cbegin(); it != s.cend(); ++it)
        EXPECT_EQ(*it, expected3[idx++]);
}

TEST(StackTest, Stack_PushPopCopy)
{
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    s.pop();
    s.push(10);
    s.push(20);

    Stack<int> copy_s(s);

    int expected_orig[] = {20, 10, 2, 1};
    int idx = 0;
    for (auto v : copy_s) EXPECT_EQ(v, expected_orig[idx++]);

    copy_s.pop();
    copy_s.pop();
    copy_s.push(99);
    copy_s.push(98);
    copy_s.push(97);

    idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_orig[idx++]);

    int expected_copy[] = {97, 98, 99, 2, 1};
    idx = 0;
    for (auto v : copy_s) EXPECT_EQ(v, expected_copy[idx++]);

    Stack<int> moved_s(std::move(copy_s));

    int expected_moved[] = {97, 98, 99, 2, 1};
    idx = 0;
    for (auto v : moved_s) EXPECT_EQ(v, expected_moved[idx++]);

    EXPECT_TRUE(copy_s.empty());

    Stack<int> s2;
    s2 = s;
    idx = 0;
    for (auto v : s2) EXPECT_EQ(v, expected_orig[idx++]);

    s2.pop();
    s2.push(42);
    s2.push(99);

    idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_orig[idx++]);

    int expected_copy_asgn[] = {99, 42, 10, 2, 1};
    idx = 0;
    for (auto v : s2) EXPECT_EQ(v, expected_copy_asgn[idx++]);


    Stack<int> s3;
    s3 = std::move(s2);
    idx = 0;
    for (auto v : s3) EXPECT_EQ(v, expected_copy_asgn[idx++]);

    EXPECT_TRUE(s2.empty());

    s3.pop();
    s3.push(77);
    int expected_final[] = {77, 42, 10, 2, 1};
    idx = 0;
    for (auto v : s3) EXPECT_EQ(v, expected_final[idx++]);
}

TEST(StackTest, Stack_IO)
{
    Stack<int> s;
    s.push(0);

    std::stringstream sin("1 2 3 4 5");
    sin >> s;

    EXPECT_EQ(s.size(), 6u);

    int expected[] = {5, 4, 3, 2, 1, 0};
    int idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected[idx++]);

    std::stringstream sout;
    sout << s;

    std::string expected_str = "5 4 3 2 1 0";
    EXPECT_EQ(sout.str(), expected_str);

    for (auto& v : s) v += 10; 

    std::stringstream sout2;
    sout2 << s;
    std::string expected_str2 = "15 14 13 12 11 10";
    EXPECT_EQ(sout2.str(), expected_str2);
}

TEST(StackTest, Stack_Algs)
{
    Stack<int> s;

    for (int i = 1; i <= 5; ++i) s.push(i);

    auto it = std::find_if(s.begin(), s.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(*it, 4);
    *it = 3;

    const Stack<int>& r = s;
    auto cit = std::find_if(r.begin(), r.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(*cit, 2);

    it = std::find_if(s.begin(), s.end(), [](int v){ return v == 0; });
    EXPECT_EQ(it, s.end());

    auto count_even = std::count_if(s.begin(), s.end(), [](int v){ return v % 2 == 0; });
    EXPECT_EQ(count_even, 1);

    std::replace_if(s.begin(), s.end(), [](int v){ return v % 2 != 0; }, 99);
    int expected_replace[] = {99, 99, 99, 2, 99};
    int idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_replace[idx++]);

    std::for_each(s.begin(), s.end(), [](int& v){ v += 1; });
    int expected_for_each[] = {100, 100, 100, 3, 100};
    idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_for_each[idx++]);
}

TEST(ContainerTest, BaseContainer)
{
    Stack<int> s;
    Queue<int> q;

    s.push(1);
    s.push(2);
    s.push(3);
    q.push(10);
    q.push(20);
    q.push(30);

    fwd_container<int>& bs = s;
    fwd_container<int>& bq = q;
    const fwd_container<int>& cs = s;
    const fwd_container<int>& cq = q;

    //bs = q; // forbidden? -> otherwise we are gonna get bad_cast

    int expected_s_after[] = {3, 2, 1}; //{30, 20, 10}; //<---- Why?? Stack is (3, 2, 1)
    int idx = 0;
    for (auto v : s) EXPECT_EQ(v, expected_s_after[idx++]);

    //bq = s; // forbidden? -> otherwise we are gonna get bad_cast
    int expected_q_after[] = {10, 20, 30};
    idx = 0;
    for (auto v : q) EXPECT_EQ(v, expected_q_after[idx++]);

    idx = 0;
    for (auto& it : bs) EXPECT_EQ(it, expected_s_after[idx++]);

    idx = 0;
    for (const auto& it : bs) EXPECT_EQ(it, expected_s_after[idx++]);

    idx = 0;
    for (auto& it : cs) EXPECT_EQ(it, expected_s_after[idx++]);

    //int expected_queue[] = {10, 20, 30}; // We dont use this array in the tests
    idx = 0;
    for (auto& it : bq) EXPECT_EQ(it, expected_q_after[idx++]);

    idx = 0;
    for (const auto& it : bq) EXPECT_EQ(it, expected_q_after[idx++]);

    idx = 0;
    for (auto& it : bq) EXPECT_EQ(it, expected_q_after[idx++]);

}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

