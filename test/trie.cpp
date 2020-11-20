#include <gtest/gtest.h>

#include <src/trie.h>

TEST(trie, insert)
{
	trie t;
	EXPECT_EQ(t.insert("hello"), true);
	EXPECT_EQ(t.insert("hi"), true);
	EXPECT_EQ(t.insert("hello"), false);
	EXPECT_EQ(t.insert("help"), true);
	EXPECT_EQ(t.insert("hilo"), true);
	EXPECT_EQ(t.insert("he"), true);
	EXPECT_EQ(t.insert("hilo"), false);
	EXPECT_EQ(t.insert("blarg"), true);
	EXPECT_EQ(t.insert("fiou23fe89237429--!!?."), true);
	
	EXPECT_EQ(t.has("hello"), true);
	EXPECT_EQ(t.has("hi"), true);
	EXPECT_EQ(t.has("help"), true);
	EXPECT_EQ(t.has("hilo"), true);
	EXPECT_EQ(t.has("he"), true);
	EXPECT_EQ(t.has("blarg"), true);
	EXPECT_EQ(t.has("fiou23fe89237429--!!?."), true);
	
	EXPECT_EQ(t.has("blar"), false);
	EXPECT_EQ(t.has("bla"), false);
	EXPECT_EQ(t.has("bl"), false);
	EXPECT_EQ(t.has("b"), false);
	EXPECT_EQ(t.has(""), false);
	EXPECT_EQ(t.has("h"), false);
	EXPECT_EQ(t.has("hel"), false);
}


